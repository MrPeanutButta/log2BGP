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

#ifndef MSG_TEXT_H
#define	MSG_TEXT_H

const char *peer_state[] = {
    "Idle",
    "Connect",
    "Active",
    "Open Sent",
    "Open Confirm",
    "Established",
    "Deleted",
    "Clearing"
};

const char *bgp_msg_types[] = {
    (const char *) NULL,
    "Open",
    "Update",
    "Notification",
    "Keepalive",
    "Router refresh"
};

const char *bgp_error_codes[] = {
    (const char *) NULL,
    "message header error, ",
    "open message error, ",
    "update message error, ",
    "hold timer expired",
    "finite state machine error",
    "(cease)"
};

const char *bgp_error_sub_header[] = {
    (const char *) NULL,
    "connection not synchronized",
    "bad message length",
    "bad message type"
};

const char *bgp_error_sub_open[] = {
    (const char *) NULL,
    "unsupported version number",
    "bad peer AS",
    "bad BGP identifier",
    "unsupported optional parameter",
    (const char *) NULL, // 5 is deprecated
    "unacceptable hold-timer",
    "unsupported capability"
};

const char *bgp_error_sub_update[] = {
    (const char *) NULL,
    "malformed attribute list",
    "unrecognized well-known attribute",
    "missing well-known attribute",
    "attribute flags error",
    "attribute length error",
    "invalid origin attribute",
    "AS routing Loop",
    "invalid NEXT_HOP attribute",
    "optional attribute error",
    "invalid network field",
    "malformed AS_PATH"
};

const char *bgp_origin_code_text[] = {
    "i", // IGP
    "e", // EGP
    "?" //INCOMPLETE (redisributed)
};

const char bgp_rib_code_text[] = {
    's', // SUPPRESSED
    'd', // DAMPED
    'h', // HISTORY
    '*', // VALID
    '>', // BEST
    'i', // INTERNAL
    'r', // RIB_FAILURE
    'S' // STALE
};

/*!syslog() - list of severity Levels:
 *0 -       Emergency: system is unusable
 *1 -       Alert: action must be taken immediately
 *2 -       Critical: critical conditions
 *3 -       Error: error conditions
 *4 -       Warning: warning conditions
 *5 -       Notice: normal but significant condition
 *6 -       Informational: informational messages
 *7 -       Debug: debug-level messages
 */
const char *c_BGP_0_EMERG = "%BGP-0-EMERG";
const char *c_BGP_1_ALERT = "%BGP-1-ALERT";
const char *c_BGP_2_CRIT = "%BGP-2-CRIT";
const char *c_BGP_3_NOTIFICATION = "%BGP-3-NOTIFICATION";
const char *c_BGP_4_WARN = "%BGP-4-WARN";
const char *c_BGP_5_NOTICE = "%BGP-5-NOTICE";
const char *c_BGP_5_ADJCHANGE = "%BGP-5-ADJCHANGE";
const char *c_BGP_6_UPDATE = "%BGP-6-UPDATE";
const char *c_BGP_6_WITHDRAW = "%BGP-6-WITHDRAW";
const char *c_BGP_7_DEBUG = "%BGP-7-DEBUG";


#endif	/* MSG_TEXT_H */

