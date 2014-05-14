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

/** BGP::notification - prepare and send notification.
 *
 * sends notification to peer.
 * XXX may need to write data about notification to peer.
 */
void BGP::notification(bgp_peer *peer,
        bgp_error_code err_code, bgp_error_subcode err_subcode) {

    unsigned char packet[2];

    packet[0] = (uint8_t) err_code;
    packet[1] = (uint8_t) err_subcode;

    send_packet(peer, packet, 2, bgp_msg_type::notification);
}

/** BGP::notification - prepare and send notification.
 *
 * sends notification to peer.
 * XXX may need to write data about notification to peer.
 */
void BGP::notification(bgp_peer *peer,
        bgp_error_code err_code, bgp_open_error_subcode err_subcode) {

    unsigned char packet[2];

    packet[0] = (uint8_t) err_code;
    packet[1] = (uint8_t) err_subcode;

    send_packet(peer, packet, 2, bgp_msg_type::notification);
}

/** BGP::decode_notification - decodes received notification.
 *
 * decodes received notification from peer.
 * logs message to syslog.
 * XXX may need to read 'data' from peer
 */
void BGP::decode_notification(bgp_peer *peer, unsigned char *packet, unsigned short length) {
    bgp_notification header;

    header.error_code = (bgp_error_code) packet[0];
    header.error_subcode = (bgp_error_subcode) packet[1];

    memcpy(header.data, packet + 2, 6);
    syslog(LOG_DEBUG, "notice code=%i, subcode=%i", int(header.error_code), int(header.error_subcode));

    switch (header.error_code) {
        case bgp_error_code::msg_head_err:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], bgp_error_sub_header[(uint8_t) header.error_subcode]);
            break;
        case bgp_error_code::open_msg_err:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], bgp_error_sub_open[(uint8_t) header.error_subcode]);
            break;
        case bgp_error_code::update_msg_err:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], bgp_error_sub_update[(uint8_t) header.error_subcode]);
            break;
        case bgp_error_code::hold_time_expired:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], "");
            break;
        case bgp_error_code::finite_state_mach_err:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], "");
            break;
        case bgp_error_code::cease:
            BGP_3_NOTIFICATION(bgp_error_codes[(uint8_t) header.error_code], "");
            break;
        default:
            break;
    }
}

