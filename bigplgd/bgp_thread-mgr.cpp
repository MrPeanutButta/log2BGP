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

#include <thread>
#include <cstring>
#include "bgp.h"
#include "bgp_thread-mgr.h"

thread_mgr::thread_mgr() {
}

thread_mgr::~thread_mgr() {
    thread_q.erase(thread_q.begin(), thread_q.end());
    thread_q.clear();
}

/** ThreadMgr::TLog - can be used for general purpose thread logging.
 *
 * logs thread messages to syslog().
 */
void thread_mgr::TLog(std::string t_log) {
    syslog(LOG_DEBUG, "thread_log: %s", t_log.c_str());
}

/** ThreadMgr::start_peer - creates a new peer thread.
 *
 * copies peer_params in to new bgp_peer and is pushed on to the vector.
 * thread is then detached.
 */
void thread_mgr::start_peer(BGP *bgp, bgp_peer *peer_params) {
    if (!bgp) return;

    thread_q.push_back(peer_t(new bgp_peer(*peer_params)));
    thread_q.back().get()->fsm = new std::thread(&BGP::init_peer, bgp, thread_q.back().get());
    thread_q.back().get()->fsm->detach();
    TLog(std::string("detach() ") + std::string(thread_q.back().get()->p_ip));
}

/** ThreadMgr::delete_peer - remove peer from vector.
 *
 * search vector for peer ipv4 address and erase it.
 */
int thread_mgr::delete_peer(bgp_peer *peer) {
    if (!peer)return -1;

    for (uint8_t i(0); i < thread_q.size(); i++) {
        if (thread_q.at(i)->bgp_id == peer->bgp_id) {
            thread_q.erase(thread_q.begin() + i);
            TLog(std::string("erase() ") + std::string(peer->p_ip));
        }
    }

    return 0;
}

/** ThreadMgr::PeerExists - Checks for the peers existence
 */
bool thread_mgr::peer_exists(const bgp_peer *peer) {
    if (!peer) return false;

    for (uint8_t i(0); i < thread_q.size(); ++i) {
        switch (peer->addr_family) {
            case AF_INET:
                if (thread_q[i]->ipv4 == peer->ipv4) return true;
                break;
#ifdef USE_AF_INET6
            case AF_INET6:
                if (thread_q[i]->lcl_sockaddr_v6.sin6_addr.s6_addr == peer->lcl_sockaddr_v6.sin6_addr.s6_addr) {
                    thread_mgr_().unlock();
                    return true;
                }
                break;
#endif
        }
    }

    return false;
}

/** ThreadMgr::peer_exists - Checks for the peers existence.
 */
bool thread_mgr::peer_exists(const std::string &peer_str) {
    if (peer_str.empty()) return false;

    for (peer_t peer : this->thread_q)
        if (!peer_str.compare(peer->p_ip))return true;

    return false;
}

/** ThreadMgr::get_peer_by_str_ip - returns pointer to peer.
 *
 * if "x.x.x.x" represents an existing peer
 * return pointer to it.
 */
bgp_peer *thread_mgr::get_peer_by_str_ip(const std::string &ip) {
    if (ip.empty()) return nullptr;

    for (peer_t peer : this->thread_q)
        if (!ip.compare(peer->p_ip)) return peer.get();

    return nullptr;
}

/** ThreadMgr::get_peer_by_uint32_t_bgp_id - returns pointer to peer
 *
 * if uint32_t represents an existing peer
 * return a pointer to it.
 */
bgp_peer *thread_mgr::get_peer_by_uint32_t_bgp_id(const uint32_t &bgp_id) {
    for (peer_t peer : this->thread_q) {
        if (peer.get() != nullptr) {
            if (peer->bgp_id == bgp_id)return peer.get();
        }
    }

    return nullptr;
}

bool thread_mgr::peer_doing_alias(const uint32_t &bgp_id) {
    bool ret_val(false);
    for (peer_t peer : this->thread_q) {
        if (peer.get() != nullptr) {
            ret_val = peer->doing_alias();
            break;
        }
    }

    return ret_val;
}