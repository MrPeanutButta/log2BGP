/*
 * flowlab
 * Copyright (C) Aaron Hebert 2012 - 2013 <aaron.hebert@gmail.com>
 *
 * flowlab is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * flowlab is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bgp.h"

/** Sends packet to peer.
 *
 * builds packet and sends to threads peer
 */
int BGP::send_packet(bgp_peer *peer, unsigned char *payload, unsigned len, bgp_msg_type msg_type) {

    if (peer->state == bgp_peer_state::idle) return 0;

    //failsafe
    if (msg_type == bgp_msg_type::update) return 0;

    bgp_header header;
    memset(&header, 0, sizeof (bgp_header));

    //message + header length
    header.len = len + 19;
    header.type = msg_type;
    memset(&header.marker, 0xff, 16);

    //write marker
    peer->write(header.marker, 1, 16);
    //big endian header length
    peer->write_formated("%c%c", header.len >> 8, header.len & 0xff);
    //write TYPE of message
    peer->write(&header.type, 1, 1);
    //write payload
    peer->write(payload, 1, len);
    //flush to send
    peer->tx_flush();

    syslog(LOG_DEBUG,
            "BGP::Send() length = %i : type = %s",
            header.len, bgp_msg_types[(uint8_t) msg_type]);

    //increment counters
    peer->tx_rx.tx += header.len;
    peer->last_IO.last_write = time(NULL);

    // increment tx counter
    switch (msg_type) {
        case bgp_msg_type::open:
            ++peer->opens.tx;
            peer->opens.last_tx = time(NULL);
            break;
        case bgp_msg_type::update:
            ++peer->update.tx;
            peer->update.last_tx = time(NULL);
            break;
        case bgp_msg_type::notification:
            ++peer->notify.tx;
            peer->notify.last_tx = time(NULL);
            break;
        case bgp_msg_type::keepalive:
            ++peer->keepaliv.tx;
            peer->keepaliv.last_tx = time(NULL);
            break;
        case bgp_msg_type::routerefresh:
            ++peer->routeref.tx;
            peer->routeref.last_tx = time(NULL);
            break;
    }

    //return the length of the message just sent. (not used)
    return header.len;
}

