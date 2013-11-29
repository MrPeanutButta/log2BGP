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

/** Builds and sends an OPEN message.
 *
 * takes the current threads (bgp_peer *) as arg
 * and sends an open to that peer
 *
 * XXX needs to insert auth values
 */
int BGP::send_open(bgp_peer *peer) {

    bgp_open *open_msg = new bgp_open;

    const uint8_t open_message_len(10);
    uint8_t open_message[open_message_len];

    open_msg->version = this->version;
    open_msg->hold_time = this->hold_time;
    open_msg->AS = this->AS;
    open_msg->bgp_id = this->bgp_id;
    open_msg->opt_param_len = 0;

    // version is 1 byte
    open_message[0] = open_msg->version;

    // AS is 2 bytes
    open_message[1] = open_msg->AS >> 8;
    open_message[2] = open_msg->AS & 0xff;

    // hold time is 2 bytes
    open_message[3] = open_msg->hold_time >> 8;
    open_message[4] = open_msg->hold_time & 0xff;

    // shift through each octet and assign that byte to open_message[]
    open_message[5] = open_msg->bgp_id & 0xff;
    open_message[6] = open_msg->bgp_id >> 8;
    open_message[7] = open_msg->bgp_id >> 16;
    open_message[8] = open_msg->bgp_id >> 24;

    // number of bytes
    open_message[9] = open_msg->opt_param_len;

    syslog(LOG_DEBUG,
            "BGP::Open() Sending AS = %i : holdtime = %i : ID = %i",
            open_msg->AS, open_msg->hold_time, open_msg->bgp_id);

    send_packet(peer, open_message, open_message_len, bgp_msg_type::open);

    peer->state = bgp_peer_state::opensent;
    syslog(LOG_DEBUG,
            "peer %s state = %s",
            peer->p_ip, peer_state[(uint8_t) peer->state]);

    delete open_msg;

    return open_message_len;
}

/** Decodes an OPEN message.
 *
 * decodes open message from threads (bgp_peer *)
 * XXX needs to perform OPEN message checks and notify peer if something is wrong
 * XXX possible race condition on 'version' and 'holdtime' during session setup
 *     if someone issues a 'show bgp neighbors'.
 */
void BGP::decode_open(bgp_peer *peer, unsigned char *packet,
        unsigned short length) {

    bgp_open open_header;
    memset(&open_header, 0, sizeof (bgp_open));

    open_header.version = packet[0];
    open_header.AS = (packet[1] << 8) + packet[2];
    open_header.hold_time = (packet[4] + packet[3]);
    open_header.bgp_id =
            (packet[5]) +
            (packet[6] << 8) +
            (packet[7] << 16) +
            (packet[8] << 24);

    open_header.opt_param_len = packet[9];

    if (open_header.version != this->version) {
        // notify of incorrect/unsupported version, close session
        this->notification(peer, bgp_error_code::open_msg_err,
                bgp_open_error_subcode::unsup_ver_num);

        peer->state = bgp_peer_state::idle;
        peer->disconnect();
        return;
    } else {
        peer->lock();
        memcpy(&peer->version, &open_header.version,
                sizeof (open_header.version));

        peer->unlock();
    }

    if (open_header.AS != peer->AS) {
        // notify of AS miss-match, close session
        this->notification(peer, bgp_error_code::open_msg_err,
                bgp_open_error_subcode::bad_peer_as);

        peer->state = bgp_peer_state::idle;
        peer->disconnect();
        return;
    } else {
        peer->lock();
        memcpy(&peer->AS, &open_header.AS, sizeof (open_header.AS));
        peer->unlock();
    }

    in_addr n_ip;
    memset(&n_ip, 0, sizeof (in_addr));
    n_ip.s_addr = open_header.bgp_id;

    if (!inet_ntoa(n_ip)) {
        // notify of bad bgp id, close session
        this->notification(peer, bgp_error_code::open_msg_err,
                bgp_open_error_subcode::bad_bgp_id);

        peer->state = bgp_peer_state::idle;
        peer->disconnect();
        return;
    } else {
        peer->lock();
        memcpy(&peer->bgp_id, &open_header.bgp_id, sizeof (open_header.bgp_id));
        strcpy(peer->b_ip, inet_ntoa(n_ip));
        peer->unlock();
    }

    peer->lock();
    memcpy(&peer->hold_time, &open_header.hold_time,
            sizeof (open_header.hold_time));
    memcpy(&peer->keep_alive, &open_header.hold_time,
            sizeof (open_header.hold_time));

    peer->keep_alive /= 3;
    // timer is in milliseconds
    //    peer->hold_timer->interval(peer->keep_alive * 1000);

    memcpy(&peer->opt_param_len, &open_header.opt_param_len,
            sizeof (open_header.opt_param_len));

    peer->state = bgp_peer_state::openconfirm;

    syslog(LOG_DEBUG,
            "BGP::DecodeOpen() version = %i : AS = %i : hold = %i : ID = %i : \
                opt_len = %i",
            peer->version, peer->AS, peer->hold_time, peer->bgp_id,
            peer->opt_param_len);

    syslog(LOG_DEBUG,
            "peer %s state = %s",
            peer->p_ip, peer_state[(uint8_t) peer->state]);

    peer->unlock();

    // this should bring up the session
    send_packet(peer, 0, 0, bgp_msg_type::keepalive);
}