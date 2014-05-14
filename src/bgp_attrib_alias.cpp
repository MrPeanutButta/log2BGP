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

#include <sstream>
#include <cstring>
#include "bgp.h"
#include "misc.h"

/** Returns string of bgp_id.
 *
 * bgp_id - BGP ID of peer (network byte order).
 */
std::string BGP::translate_bgp_id(const uint32_t bgp_id) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::string __s(ip_to_string(bgp_id));

    if (peer == nullptr) return __s;

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::bgp_id) {
            __s = attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}

/** deprecated.
 */
std::string BGP::translate_origin(const uint32_t bgp_id, const uint8_t origin) {

    std::string bgp_origin_code_string[] = {
        "igp",
        "egp",
        "incomplete"
    };

    std::string __s(bgp_origin_code_string[origin]);

    return __s;
}

/* Alias AS value.
 *
 * returns string representation of AS number (host byte order).
 */
std::string BGP::translate_as_path(const uint32_t bgp_id,
        const uint32_t as_path) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::stringstream s_s;
    s_s.clear();
    s_s << as_path;
    std::string __s(s_s.str());

    // append label to AS number
    if (peer == nullptr) return s_s.str();

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::as_path &&
                !memcmp(&attrib.value, &as_path, sizeof (uint32_t))) {
            __s += attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}

/** Alias NEXT_HOP.
 *
 * returns string representation of NEXT_HOP.
 *
 * bgp_id - BGP ID of peer (network byte order).
 * next_hop - next hop IP address (network byte order).
 */
std::string BGP::translate_ipv4_next_hop(const uint32_t bgp_id,
        const uint32_t next_hop) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::string __s(ip_to_string(next_hop));

    if (peer == nullptr) {
        return __s;
    }

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::next_hop &&
                !memcmp(&attrib.value, &next_hop, sizeof (uint32_t))) {
            __s = attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}

/** Alias MULTI_EXIT_DISC.
 *
 * returns string representation of MULTI_EXIT_DISC.
 *
 * bgp_id - BGP ID of peer (network byte order).
 * med - MULTI_EXIT_DISC value (host byte order).
 */
std::string BGP::translate_med(const uint32_t bgp_id, const uint32_t med) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::stringstream s_s;
    s_s.clear();
    s_s << med;
    std::string __s(s_s.str());

    if (peer == nullptr) return __s;

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::multi_exit_disc &&
                !memcmp(&attrib.value, &med, sizeof (uint32_t))) {
            __s = attrib.printable_attrib;

            return __s;
        }
    }

    return __s;
}

/** Alias LOCAL_PREF.
 *
 * returns string representation of LOCAL_PREF.
 *
 * bgp_id - BGP ID of peer (network byte order).
 * local_pref - LOCAL_PREF value (host byte order).
 */
std::string BGP::translate_local_pref(const uint32_t bgp_id,
        const uint32_t local_pref) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::stringstream s_s;
    s_s.clear();
    s_s << local_pref;
    std::string __s(s_s.str());

    if (peer == nullptr) return __s;

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::local_pref &&
                !memcmp(&attrib.value, &local_pref, sizeof (uint32_t))) {
            __s = attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}

/** Alias AGGREGATOR.
 *
 * returns string representation of AGGREGATOR.
 *
 * bgp_id - BGP ID of peer (network byte order).
 * aggr_seg - IPv4 AGGREGATOR value (network byte order).
 */
std::string BGP::translate_aggregator(const uint32_t bgp_id,
        const uint32_t aggr_seg) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::string __s(ip_to_string(aggr_seg));

    if (peer == nullptr) return __s;

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::aggregator &&
                !memcmp(&attrib.value, &aggr_seg, sizeof (uint32_t))) {
            __s += attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}

/** Alias COMMUNITY.
 *
 * returns string representation of COMMUNITY.
 *
 * bgp_id - BGP ID of peer (network byte order).
 * value -  COMMUNITY value (host byte order).
 */
std::string BGP::translate_community_value(const uint32_t bgp_id,
        const uint16_t value) {

    bgp_peer *peer = nullptr;

    if (this->peer_doing_alias(bgp_id)) {
        peer = this->get_peer_by_uint32_t_bgp_id(bgp_id);
    }

    std::stringstream s_s;
    s_s.clear();
    s_s << value;
    std::string __s(s_s.str());

    if (peer == nullptr) return __s;

    for (translate_attribs_base &attrib : peer->translate_attribs) {
        if (attrib.code == bgp_path_attrib::community &&
                !memcmp(&attrib.value, &value, sizeof (uint32_t))) {
            __s = attrib.printable_attrib;
            return __s;
        }
    }

    return __s;
}
