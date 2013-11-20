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

#include <string>
#include <cstring>
#include <algorithm>
#include "bgp_peer.h"
#include "misc.h"

bgp_peer::bgp_peer() :
tcp_client(), ipv4(0), bgp_id(0), update_src_ip(0), version(4), hold_time(180),
min_hold_time(0), keep_alive(60), update_freq(1), AS(0), opt_param_len(0),
capabilities_advertisement(0), table_version(0), in_q(0), out_q(0), up_since(0),
state(bgp_peer_state::idle), addr_family(0), /*hold_timer(new Timer),*/
update_timer(new Timer), is_aliasing(false) {

    // zero out peer struct counters
    memset(&tx_rx, 0, sizeof (bytes_tx_rx));
    memset(&last_IO, 0, sizeof (read_write));
    memset(&keepaliv, 0, sizeof (bytes_tx_rx));
    memset(&opens, 0, sizeof (bytes_tx_rx));
    memset(&notify, 0, sizeof (bytes_tx_rx));
    memset(&routeref, 0, sizeof (bytes_tx_rx));
    memset(&update, 0, sizeof (bytes_tx_rx));
    memset(&implicit_withdraw, 0, sizeof (byte_tx_rx_current));
    memset(&explicit_withdraw, 0, sizeof (byte_tx_rx_current));

    fsm = nullptr;
}

// copy constructor.

bgp_peer::bgp_peer(const bgp_peer& orig) : bgp_peer() {

    this->AS = orig.AS;
    this->addr_family = orig.addr_family;
    memcpy(this->p_ip, orig.p_ip, INET_ADDRSTRLEN);
}

bgp_peer::~bgp_peer() {

    update_timer->Stop(); // process update queue

    if (this->fsm != nullptr) {
        delete this->fsm;
        this->fsm = nullptr;
    }

    if (update_timer != nullptr) {
        delete this->update_timer;
        this->update_timer = nullptr;
    }

    try {
        translate_attribs.clear();
    } catch (...) {
    }

}

bool alias_sort(const translate_attribs_base &a, const translate_attribs_base &b) {
    return a.code < b.code;
}

/** Adds values to be aliased by splunk_dumper
 */
std::string bgp_peer::set_translation(bgp_path_attrib code, uint32_t &match_value, std::string &new_value) {
    //if (code > 8 || code < 0) return "attribute code not available for translation\n";

    this->lock();

    int size = this->translate_attribs.size();

    for (int i = 0; i < size; i++) {
        if (this->translate_attribs[i].code == code
                && this->translate_attribs[i].value == match_value) {

            this->translate_attribs.erase(this->translate_attribs.begin() + i);
        }
    }

    translate_attribs_base attrib = {code, match_value, new_value};
    this->translate_attribs.emplace_back(attrib);

    sort(this->translate_attribs.begin(), this->translate_attribs.end(), alias_sort);
    this->is_aliasing = true;

    this->unlock();
    return "";
}
