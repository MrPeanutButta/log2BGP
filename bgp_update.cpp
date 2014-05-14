/*
 * BigPlg
 * Copyright (C) Aaron Hebert 2012 - Present <aaron.hebert@gmail.com>
 *
 * BigPlg is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BigPlg is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bgp.h"
#include "bgp_peer.h"

/** Called on a schedule to process updates in queue.
 *
 * will pop the top update in queue and decrement the in_q counter for peer.
 * parses BGP attributes.
 * generates subnet range after parsing updates.
 * pushes to ADJ-RIB-in.
 *
 * bgp - BGP instance.
 * peer - Peer that received the update.
 */
int BGP::proc_in_q(bgp_peer *peer) {

#ifndef BGP_UNIT_TEST
    if (this->hold_timer_expired(peer)) return 0;
#endif

    if (peer->in_queue.empty()) return 0;

    bgp_attribute attrib;
    bgp_update *update = nullptr;
    update_data u_data;

    //length of routes to withdraw
    uint16_t unfeasable_routes_len(0);
    //path attribute byte len
    uint16_t path_attrib_len(0);
    //length of packet
    uint16_t p_len(0);
    //how many bytes we have read
    uint16_t read_len(0);

    //packet index position
    int i(0);

    while (!peer->in_queue.empty()) {

        memset(&attrib, 0, sizeof (bgp_attribute));
        //memset(&update, 0, sizeof (bgp_update));
        update = new bgp_update;
        p_len = peer->in_queue.front().length;

        //alloc +5 for padding
        unsigned char *packet = new (std::nothrow)
                unsigned char [p_len + 5];

        if (packet == nullptr) {
            syslog(LOG_CRIT,
                    "malloc of packet buffer from in queue failed, peer=%s",
                    peer->p_ip);
            return -1;
        }

        //zero and copy packet from queue
        memset(packet, 0, p_len + 5);
        memcpy(packet, peer->in_queue.front().packet, p_len);

        peer->lock_in_queue();
        peer->in_queue.pop();
        peer->in_q--;
        peer->unlock_in_queue();

        //set position to beginning
        i = 0;
        unfeasable_routes_len = (packet[i] << 8) + packet[i + 1];
        ++i;

        /* if unfeasable_routes_len !=0
         * this is a withdraw */
        if (unfeasable_routes_len != 0) {
            ++i;
            parse_nlri(*peer, update, i, packet, nlri_parse::withdraw);
            //continue with next in queue
            continue;
        }

        //2 byte attrib length
        path_attrib_len = (packet[i + 1] << 8) + packet[i + 2];

        //move up 2 bytes
        i += 2;
        read_len = 0;

        for (; read_len < path_attrib_len;) {
            attrib.flags = packet[++i];
            attrib.type_code = (bgp_path_attrib) packet[++i];
            attrib.length = packet[++i];

            read_len += 3;

            switch (attrib.type_code) {
                case bgp_path_attrib::origin: if (attrib.length == 0) break;

                    update->origin = packet[++i];
                    ++read_len;

                    break;
                case bgp_path_attrib::as_path: if (attrib.length == 0) break;
                    memset(&u_data, 0, sizeof (update_data));
                    u_data.offset = i;

                    parse_as_path(u_data, update, packet);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::next_hop: if (attrib.length == 0)break;
                    // keep next hop big endian
                    memcpy(&update->ipv4_next_hop,
                            &packet[i + 1],
                            attrib.length);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::multi_exit_disc: if (attrib.length == 0)break;
                    // little endian 4 bytes
                    update->med =
                            (
                            (packet[i + 1] << 24) +
                            (packet[i + 2] << 16) +
                            (packet[i + 3] << 8) +
                            (packet[i + 4])
                            );

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::local_pref: if (attrib.length == 0)break;
                    // little endian 4 bytes
                    update->local_pref =
                            (
                            (packet[i + 1] << 24) +
                            (packet[i + 2] << 16) +
                            (packet[i + 3] << 8) +
                            (packet[i + 4])
                            );

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::atomic_aggregate:
                    update->atomic_aggregate = 1;
                    //this attrib is zero bytes
                    break;
                case bgp_path_attrib::aggregator: if (attrib.length == 0) break;

                    update->aggregator = new aggregator_segment;
                    memset(update->aggregator, 0, sizeof (aggregator_segment));

                    // little endian AS
                    update->aggregator->as =
                            (
                            (packet[i + 1] << 8) + packet[i + 2]
                            );

                    // keep origin IP big endian
                    memcpy(&update->aggregator->origin,
                            &packet[i + 3],
                            attrib.length - 2);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::community: if (attrib.length == 0) break;

                    memset(&u_data, 0, sizeof (update_data));
                    u_data.offset = i;
                    u_data.attrib.length = attrib.length;

                    parse_community(u_data, update,
                            packet);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::originator: if (attrib.length == 0) break;

                    i += attrib.length;
                    read_len += attrib.length;
                    syslog(LOG_DEBUG,
                            "unimplemented attribute code %u, length=%u",
                            (uint8_t) attrib.type_code, (uint8_t) attrib.length);
                    break;
                case bgp_path_attrib::cluster_list: if (attrib.length == 0) break;

                    i += attrib.length;
                    read_len += attrib.length;
                    syslog(LOG_DEBUG,
                            "unimplemented attribute code %u, length=%u",
                            (uint8_t) attrib.type_code, (uint8_t) attrib.length);
                    break;
                case bgp_path_attrib::mp_reach_nlri: if (attrib.length == 0) break;

                    i += attrib.length;
                    read_len += attrib.length;
                    syslog(LOG_DEBUG,
                            "unimplemented attribute code %u, length=%u",
                            (uint8_t) attrib.type_code, (uint8_t) attrib.length);
                    break;
                case bgp_path_attrib::mp_unreach_nlri: if (attrib.length == 0) break;

                    i += attrib.length;
                    read_len += attrib.length;
                    syslog(LOG_DEBUG,
                            "unimplemented attribute code %u, length=%u",
                            (uint8_t) attrib.type_code, (uint8_t) attrib.length);

                    break;
                case bgp_path_attrib::as4_path: if (attrib.length == 0) break;

                    memset(&u_data, 0, sizeof (update_data));
                    u_data.offset = i;

                    parse_as4_path(u_data, update, packet);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                case bgp_path_attrib::as4_aggregator: if (attrib.length == 0) break;

                    update->as4_aggregator = new aggregator_segment;
                    memset(update->as4_aggregator, 0, sizeof (aggregator_segment));

                    update->as4_aggregator->as =
                            (
                            (packet[i + 1] << 24) +
                            (packet[i + 2] << 16) +
                            (packet[i + 3] << 8) +
                            (packet[i + 4])
                            );
                    memcpy(&update->as4_aggregator->origin,
                            &packet[i + 5],
                            attrib.length - 4);

                    i += attrib.length;
                    read_len += attrib.length;

                    break;
                default:

                    read_len += attrib.length;
                    i += attrib.length;

                    syslog(LOG_DEBUG,
                            "unknown attribute code %u, length=%u",
                            (uint8_t) attrib.type_code, (uint8_t) attrib.length);

                    break;
            }//switch
        }//for

        update->addr_family = AF_INET;

        //align to nlri segment
        i = (path_attrib_len + 4);
        parse_nlri(*peer, update, i, packet, nlri_parse::update);

        delete [] packet;
        delete update;
    }


    return 0;
}

/** Parse COMMUNITY AS:VALUE.
 *
 * data - attribute data and bit offset of packet.
 * update - update message with resulting attributes.
 * packet - update being parsed.
 */
void BGP::parse_community(update_data &data, bgp_update *update,
        const unsigned char *packet) {

    update->community = new community_segment[((data.attrib.length / 4))];
    update->com_seg_length = (data.attrib.length / 4);
    memset(update->community, 0, sizeof (*update->community));

    for (int i(0); i < (data.attrib.length / 4); ++i) {
        update->community[i].as =
                ((packet[data.offset + 1] << 8) + packet[data.offset + 2]);
        update->community[i].value =
                ((packet[data.offset + 3] << 8) + packet[data.offset + 4]);
        data.offset += 4;
    }

}

/** Parse 4 byte AS4_PATH.
 *
 * data - attribute data and bit offset of packet.
 * update - update message with resulting attributes.
 * packet - update being parsed.
 */
void BGP::parse_as4_path(update_data &data, bgp_update *update,
        const unsigned char *packet) {

    update->as4_path = new as_path_segment;
    update->as4_path->seg_type = (bgp_as_path_type) packet[++data.offset];
    update->as4_path->length = (uint8_t) packet[++data.offset];

    update->as4_path->seg_value = new uint32_t[update->as4_path->length];
    memset(update->as4_path->seg_value, 0, update->as4_path->length);

    for (int as(0); as < update->as4_path->length; ++as) {
        update->as4_path->seg_value[as] =
                (
                (packet[data.offset + 1] << 24) +
                (packet[data.offset + 2] << 16) +
                (packet[data.offset + 3] << 8) +
                (packet[data.offset + 4])
                );
        data.offset += 4;
    }
}

/** Parse 2 byte AS_PATH.
 *
 * data - update data including byte position.
 * update - update message with resulting attributes.
 * packet - update being parsed.
 */
void BGP::parse_as_path(update_data &data, bgp_update *update,
        const unsigned char *packet) {

    update->as_path = new as_path_segment;
    update->as_path->seg_type = (bgp_as_path_type) packet[++data.offset];
    update->as_path->length = (uint8_t) packet[++data.offset];

    update->as_path->seg_value = new uint32_t[update->as_path->length];
    memset(update->as_path->seg_value, 0, update->as_path->length);

    for (int as(0); as < update->as_path->length; ++as) {
        update->as_path->seg_value[as] =
                (
                (packet[data.offset + 1] << 8) + packet[data.offset + 2]
                );

        data.offset += 2;
    }

}

#include "misc.h"
#include <arpa/inet.h>

/** Returns the highest IP address in subnet range.
 *
 * assuming 'classless', else returns broadcast.
 *
 * @param[in] prefix - network subnet (network byte order).
 * @param[in] length - CIDR bit length.
 * @return - highest possible IP address (network byte order).
 */
uint32_t get_ipv4_high_range(const uint32_t &prefix, const uint8_t &length) {
    uint32_t net_mask(0xFFFFFFFF << (32 - length));
    uint32_t temp_range(ntohl(prefix));

    temp_range |= (~net_mask);

    return htonl(temp_range);
}

/** Parse prefix and length.
 *
 * bgp - BGP instance.
 * peer - bgp_peer associated with this update.
 * update - updated attributes.
 * offset - nlri start position in packet[].
 * packet - update packet.
 * FLAG - withdraw or update.
 */
void BGP::parse_nlri(bgp_peer &peer, bgp_update *update,
        int &offset, const unsigned char *packet, nlri_parse FLAG) {

    size_t byte_length(0);

    /* packet[i] is expected to be a prefix_length
     * if == 0 then we've reached the end of the update */
    while (packet[offset] != 0) {

        memset(&update->nlri, 0, sizeof (bgp_nlri));
        update->nlri.prefix_length = packet[offset++];
        byte_length = (update->nlri.prefix_length / 8);

        // if len mod 8 != 0 then we need to increment by 1 byte
        if (update->nlri.prefix_length % 8)byte_length++;

        memcpy(&update->nlri.prefix, &packet[offset], byte_length);
        memset(&update->nlri.prefix_highest, 0, sizeof (uint32_t));

        // generate the highest address in range
        update->nlri.prefix_highest = get_ipv4_high_range(
                update->nlri.prefix, update->nlri.prefix_length
                );

        // tag bgp id to all nlri to assist withdraws
        memcpy(&update->nlri.bgp_id, &peer.bgp_id, sizeof (uint32_t));

        switch (FLAG) {
            case nlri_parse::update:
                update->nlri.age = time(NULL);
                // push nlri_tmp to copy ctor
                this->push_adj_rib_in_q(update);
                break;
            case nlri_parse::withdraw:
                update->nlri.last_withdraw_time = time(NULL);
                // push nlri_tmp to withdraw queue
                this->push_adj_rib_in_wdraw_q(update);
                break;
        }

        offset += (byte_length);
    }
}