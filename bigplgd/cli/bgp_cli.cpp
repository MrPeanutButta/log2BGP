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

#include <iomanip>
#include <fstream>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <time.h>
#include "bgp_msg.h"
#include "bgp.h"
#include "misc.h"
#include "bgp_database.h"

#define right_5 std::setw(5) << std::setiosflags(std::ios::right)
#define right_12 std::setw(12) << std::setiosflags(std::ios::right)
#define left_12 std::setw(12) << std::setiosflags(std::ios::left)

/** Set bgp_id.
 *
 * checks if x.x.x.x is valid and sets bgp_id.
 *
 * bgp - BGP instance.
 * tokens - vector of string values.
 * @return returns string to be written to TCP server socket.
 */
std::string BGP::set_bgp_id(BGP *bgp, std::vector<std::string> &tokens) {
    in_addr n_ip;
    std::string ret_val = "";

    if (inet_aton(tokens[1].c_str(), &n_ip) != -1) {
        bgp->set_id(n_ip.s_addr);
        syslog(LOG_DEBUG, "BGP::set_bgp_id() ID = %s", bgp->bgp_id_ip());
    } else ret_val = "% " + tokens[1] + " is not a valid bgp identifier\n";

    tokens.clear();

    return ret_val;
}

std::string BGP::display_entry(BGP *bgp, bgp_update *entry) {

    if (entry == nullptr) {
        return "entry == nullptr";
    }

    std::stringstream s_s;
    s_s.clear();

    char t_buff[80];
    strftime(t_buff, 80, "%c", gmtime(&entry->nlri.age));
    s_s << "\tLast Modified: " << t_buff << " (UTC)";

    s_s << "\n\tPath: ";

    if (entry->as_path != nullptr) {
        for (uint8_t i(0); i < entry->as_path->length; ++i) {
            s_s << bgp->translate_as_path(entry->nlri.bgp_id,
                    entry->as_path->seg_value[i]);
            s_s << " ";
        }
    }

    if (entry->as4_path != nullptr) {
        for (uint8_t i(0); i < entry->as4_path->length; ++i) {
            s_s << bgp->translate_as_path(entry->nlri.bgp_id,
                    entry->as4_path->seg_value[i]);
            s_s << " ";
        }
    }

    s_s << bgp_origin_code_text[entry->origin];

    s_s << "\n\tNext hop: ";
    s_s << bgp->translate_ipv4_next_hop(
            entry->nlri.bgp_id, entry->ipv4_next_hop
            );
    s_s << " ";

    s_s << "Metric: ";
    s_s << bgp->translate_med(entry->nlri.bgp_id, entry->med);
    s_s << " ";

    s_s << "Local Pref: ";
    s_s << bgp->translate_local_pref(entry->nlri.bgp_id, entry->local_pref);
    s_s << " ";

    if (entry->atomic_aggregate)
        s_s << " Atomic Aggregate: true";

    if (entry->aggregator != nullptr) {
        s_s << "\n\tAggregator Origin: ";
        s_s << bgp->translate_aggregator(entry->nlri.bgp_id,
                entry->aggregator->origin);

        s_s << " Aggregator AS: ";
        s_s << bgp->translate_as_path(entry->nlri.bgp_id,
                entry->aggregator->as);
    }

    if (entry->as4_aggregator != nullptr) {
        s_s << "\n\tAS4 Aggregator Origin: ";
        s_s << bgp->translate_aggregator(entry->nlri.bgp_id,
                entry->as4_aggregator->origin);
        s_s << " Aggregator AS4: ";
        s_s << bgp->translate_as_path(entry->nlri.bgp_id,
                entry->as4_aggregator->as);
    }

    if (entry->com_seg_length > 0) {
        if (entry->community != nullptr) {
            s_s << "\n\tCommunity: ";
            for (uint8_t i(0); i < entry->com_seg_length; ++i) {
                s_s << bgp->translate_as_path(entry->nlri.bgp_id,
                        entry->community[i].as);
                s_s << ":";
                s_s << bgp->translate_community_value(entry->nlri.bgp_id,
                        entry->community[i].value);
                s_s << " ";
            }
        }
    }

    return s_s.str();
}
/** Search for IPv4 prefix.
 *
 * searches for matching subnet to IPv4 address.
 *
 * bgp - BGP instance.
 * tokens - vector of string values.
 * @return returns string to be written to TCP server socket.
 */
std::string BGP::show_bgp_prefix(BGP *bgp, std::vector<std::string> &tokens) {

    in_addr n_ip;
    std::stringstream s_s;
    bool dump_history(false);
    records ret_rec;

    s_s.clear();

    if (!inet_aton(tokens[3].c_str(), &n_ip)) {
        tokens.clear();
        return s_s.str();
    }

    // container to hold matches
    std::vector<bgp_update> prefix_matches;

    if (tokens.size() == 5) {
        if (!tokens[4].compare(0, tokens[4].length(), "history")) {
            dump_history = true;
        }
    }

    /* if we don't lock and the table is active removing/inserting
     * routes, SEGFAULT is expected to occur */
    bgp->lock_adj_rib_in();

    // search for all matches
    for (const auto &entry : bgp->get_adj_rib_in()) {
        // if n_ip.s_addr is in subnet range, it's a match
        if (memcmp(
                &entry.second.nlri.prefix, &n_ip.s_addr, sizeof (uint32_t)
                ) <= 0 &&
                memcmp(
                &entry.second.nlri.prefix_highest, &n_ip.s_addr, sizeof (uint32_t)
                ) >= 0) {
            // save copy of match
            prefix_matches.emplace_back(entry.second);
        }
    }

    bgp->unlock_adj_rib_in();

    if (prefix_matches.size() == 0) {
        tokens.clear();
        s_s << "% Network not in table\n";
        return s_s.str();
    }

    s_s << std::endl;

    s_s << "Entries: (" << prefix_matches.size() <<
            " available)\n[c] = current";

    if (dump_history)s_s << " [h] = historical\n";
    else s_s << '\n';

    for (bgp_update &entry : prefix_matches) {
        // display matches
        s_s << "\nBGP routing table entry for " <<
                ip_to_string(entry.nlri.prefix) <<
                '/' << (int) entry.nlri.prefix_length <<
                " : (" << ip_to_string(entry.nlri.prefix) <<
                " - " << ip_to_string(entry.nlri.prefix_highest) << ")\n";
        s_s << "Received from " << ip_to_string(entry.nlri.bgp_id);

        if (bgp->peer_doing_alias(entry.nlri.bgp_id)) {
            s_s << "(" << bgp->translate_bgp_id(entry.nlri.bgp_id) << ")";
        }

        s_s << "\n[c]" << display_entry(bgp, &entry);

        if (dump_history) {
            ret_rec = bgp->db_get_entries(bgp, entry);

            for (bgp_update &rec : ret_rec) {
                s_s << "\n\n[h]" << display_entry(bgp, &rec);
            }

        }

        s_s << std::endl;
    }

    s_s << std::endl;

    return s_s.str();
}

std::string BGP::show_bgp_prefix_w_len(BGP *, std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::proto_bgp_toggle_update_dump(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    if (tokens[1] == "on") {
        bgp->dumper_on();
        s_s << "% Warning: with this option enabled, performance " <<
                "will be degraded during initial convergence\n";
    } else if (tokens[1] == "off")bgp->dumper_off();

    tokens.clear();

    return s_s.str();
}
/** Initialize peer defaults & finite state machine.
 *
 * initializes peer struct and calls InitPeer to enter
 * FSM and begin peer management
 *
 * should enter IDLE state and open socket
 */
std::string BGP::proto_bgp_neighbor(BGP *bgp, std::vector<std::string> &tokens) {
    bgp_peer *peer = new bgp_peer();
    sockaddr_in sa_v4;
#ifdef USE_AF_INET6
    sockaddr_in6 sa_v6;
#endif
    memset(&sa_v4, 0, sizeof (sa_v4));
#ifdef USE_AF_INET6
    memset(&sa_v6, 0, sizeof (sa_v6));
#endif

    // check IPv4
    if (inet_pton(AF_INET, tokens[1].c_str(), &(sa_v4.sin_addr)) > 0) {

        // we have ipv4 peer
        peer->addr_family = AF_INET;
        peer->ipv4 = sa_v4.sin_addr.s_addr;
        inet_ntop(AF_INET, &sa_v4.sin_addr, peer->p_ip, INET_ADDRSTRLEN);

    }
#ifdef USE_AF_INET6
        // check IPv6
    else if (inet_pton(AF_INET6, tokens[1].c_str(), &(sa_v6.sin6_addr)) > 0) {
        peer.addr_family = AF_INET6;
        peer.lcl_sockaddr_v6.sin6_family = AF_INET6;

        memcpy(&peer.lcl_sockaddr_v6.sin6_addr, &sa_v6.sin6_addr, sizeof (sa_v6.sin6_addr));
        peer.lcl_sockaddr_v6.sin6_port = htons(bgp->Port());
        inet_ntop(AF_INET6, &sa_v6.sin6_addr, peer.p_ip, INET6_ADDRSTRLEN);

    }
#endif

    std::stringstream s_s;
    s_s.clear();

    if (bgp->peer_exists(peer)) {
        s_s << "% Neighbor " << peer->p_ip << " already exists in table\n";
        delete peer;
        return s_s.str();
    }

    // authentication would be packed here

    // set peer AS and default hold
    peer->AS = atoi(tokens[3].c_str());

    bgp->start_peer(bgp, peer);

    syslog(LOG_DEBUG, "BGP::proto_bgp_neighbor() Thread count = %i : Peer = %s", bgp->peer_count(), peer->p_ip);

    delete peer;
    tokens.clear();

    return s_s.str();
}
/** General show commands.
 *
 * executes various show commands
 */
std::string BGP::Show(BGP *bgp, std::vector<std::string> &tokens) {
    return "";
}
/** Redirects peer data to stringstream.
 *
 * flips through the thread_q and prints peer stats
 * output is redirected via stringstream.
 * XXX need to add single peer lookups e.g show ip bgp neighbors <x.x.x.x>
 */
std::string BGP::show_bgp_neighbors(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    int count = bgp->peer_count();

    if (count == 0) {
        s_s << "% Peer table is empty\n";
        tokens.clear();
        return s_s.str();
    }

    bgp_peer *peer = nullptr;

    for (int i = 0; i < count; ++i) {

        peer = bgp->peer_at(i);

        s_s << "\nBGP neighbor is " << peer->p_ip << ", remote AS " << peer->AS;
        if (bgp->ASN() == peer->AS)
            s_s << ", internal link\n";
        else
            s_s << ", external link\n";

        s_s << "  BGP version " << peer->version << ", remote router ID " <<
                ((peer->doing_alias()) ? bgp->translate_bgp_id(peer->bgp_id) :
                (((peer->bgp_id ? peer->b_ip : "NOT_SET")))) << std::endl

                << "  BGP state = " << peer_state[(uint8_t) peer->state];

        if (peer->state == bgp_peer_state::established) s_s << ", up for " <<
                time_hh_mm_ss((peer->up_since > 0 ?
                (time(NULL) - peer->up_since) : 0));

        s_s << "\n  Last read " <<
                time_hh_mm_ss((peer->last_IO.last_read > 0 ?
                (time(NULL) - peer->last_IO.last_read) : 0)) << ", last write "
                << time_hh_mm_ss((peer->last_IO.last_write > 0 ?
                (time(NULL) - peer->last_IO.last_write) : 0))
                << ", hold time is " << peer->hold_time
                << ", keepalive interval is " << peer->keep_alive << " seconds\n"

                << "  Minimum holdtime from neighbor is " << peer->min_hold_time << " seconds\n"

                << "  Message statistics:\n" <<
                "    InQ depth is " << peer->in_q <<
                "\n    OutQ depth is " << peer->out_q <<
                "\n\t\t\tSent\t\tRcvd\n" <<
                "    Opens:\t\t   " << right_5 << peer->opens.tx << "\t   " << right_5 << peer->opens.rx <<
                "\n    Notifications:\t   " << right_5 << peer->notify.tx << "\t   " << right_5 << peer->notify.rx <<
                "\n    Updates:\t\t   " << right_5 << peer->update.tx << "\t   " << right_5 << peer->update.rx <<
                "\n    Keepalives:\t\t   " << right_5 << peer->keepaliv.tx << "\t   " << right_5 << peer->keepaliv.rx <<
                "\n    Route Refresh:\t   " << right_5 << peer->routeref.tx << "\t   " << right_5 << peer->routeref.rx <<

                "\n    Total:\t\t   " <<
                right_5 << (peer->opens.tx
                + peer->notify.tx
                + peer->update.tx
                + peer->keepaliv.tx
                + peer->routeref.tx) <<
                "\t   " <<
                right_5 << (peer->opens.rx
                + peer->notify.rx
                + peer->update.rx
                + peer->keepaliv.rx
                + peer->routeref.rx);

        s_s << "\n\n  Prefix activity:" << std::setw(13) << std::setiosflags(std::ios::right) <<
                "\tSent\t\tRcvd\n" <<
                "    Prefixes Current: " << std::setw(10) << std::setiosflags(std::ios::right) <<
                peer->prefixes.current.tx << std::setw(16) << std::setiosflags(std::ios::right) <<
                peer->prefixes.current.rx << std::endl <<
                "    Prefixes Total:   " << std::setw(10) << std::setiosflags(std::ios::right) <<
                peer->prefixes.tx << std::setw(16) << std::setiosflags(std::ios::right) <<
                peer->prefixes.rx << std::endl <<
                "    Implicit Withdraw:" << std::setw(10) << std::setiosflags(std::ios::right) <<
                peer->implicit_withdraw.tx << std::setw(16) << std::setiosflags(std::ios::right) <<
                peer->implicit_withdraw.rx << std::endl <<
                "    Explicit Withdraw:" << std::setw(10) << std::setiosflags(std::ios::right) <<
                peer->explicit_withdraw.tx << std::setw(16) << std::setiosflags(std::ios::right) <<
                peer->explicit_withdraw.rx << std::endl;


        s_s << "\n\n  TCP RCV socket size (bytes)\n\n";
        s_s << "\t\t\tSystem\t\tConfig\n"
                << "    \t\t\t   " << right_5 << peer->get_tcp_rx_buffer_size()
                << "\t   " << right_5 << peer->get_ref_tcp_rx_socket_size();

        s_s << "\n\n  TCP SND socket size (bytes)\n\n";
        s_s << "\t\t\tSystem\t\tConfig\n"
                << "    \t\t\t   " << right_5 << peer->get_tcp_tx_buffer_size()
                << "\t   " << right_5 << peer->get_ref_tcp_tx_socket_size() << std::endl;

    }

    s_s << "\n\n";
    tokens.clear();

    return s_s.str();
}
/** Write config to /etc/flowlab/bgpd/bgpd.conf.
 *
 * collects BGP specific data and writes to config
 */
std::string BGP::write_configs(BGP *bgp, std::vector<std::string> &tokens) {
    std::string ret_val = "[FAILED]";
    std::ofstream config("/etc/flowlab/bgpd/bgpd.conf", std::ios::out);

    if (config.fail()) {
        config.close();
        return ret_val;

    }

    config << "!\n";
    config << "protocol bgp " << bgp->ASN() << std::endl;
    if (bgp->bgp_id_ip())config << " bgp-id " << bgp->bgp_id_ip() << std::endl;
    if (bgp->dump_updates())config << " dumper on\n!\n";

    bgp_peer *peer = nullptr;

    for (int i(0); i < bgp->peer_count(); ++i) {

        peer = bgp->peer_at(i);

        peer->lock();
        config << "  !\n";
        config << "  neighbor " << peer->p_ip << " remote-as " << peer->AS << std::endl;
        config << "  neighbor " << peer->p_ip
                << " tx-buffer " << peer->get_ref_tcp_tx_socket_size()
                << ' ' << peer->get_ref_bgp_tx_buffer_size() << std::endl;
        config << "  neighbor " << peer->p_ip
                << " rx-buffer " << peer->get_ref_tcp_rx_socket_size()
                << ' ' << peer->get_ref_bgp_rx_buffer_size() << std::endl;

        udp_destinations::const_iterator __it = bgp->get_syslog_config().find(peer->bgp_id);

        if (__it != bgp->get_syslog_config().end()) {
            config << "neighbor " << peer->p_ip << " logging host " <<
                    __it->second.host() << ' ' << __it->second.port() << std::endl;
        }

        if (peer->doing_alias()) {
            for (translate_attribs_base &alias : peer->translate_attribs) {
                config << "  neighbor " << peer->p_ip << " attribute ";
                switch (alias.code) {
                    case bgp_path_attrib::bgp_id:
                        config << "bgp-id alias " << alias.printable_attrib << std::endl;
                        break;
                    case bgp_path_attrib::origin:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::as_path:
                        config << "as-path alias " << alias.value
                                << ' ' << alias.printable_attrib << std::endl;
                        break;
                    case bgp_path_attrib::next_hop:
                        config << "next-hop alias ipv4 " << ip_to_string(alias.value)
                                << ' ' << alias.printable_attrib << std::endl;
                        break;
                    case bgp_path_attrib::multi_exit_disc:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::local_pref:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::atomic_aggregate:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::aggregator:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::community:
                        config << "community alias " << alias.value
                                << ' ' << alias.printable_attrib << std::endl;
                        break;
                    case bgp_path_attrib::originator:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::cluster_list:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::mp_reach_nlri:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::mp_unreach_nlri:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::as4_path:
                        // XXX not implemented.
                        break;
                    case bgp_path_attrib::as4_aggregator:
                        // XXX not implemented.
                        break;
                }
            }
        }

        peer->unlock();
    }

    config << "!\n";
    config.close();
    tokens.clear();

    ret_val = "[OK]";
    return ret_val;
}

std::string prefixv4_wlen_print(const bgp_nlri &nlri) {
    char buffer[INET_ADDRSTRLEN];
    memset(&buffer, 0, INET_ADDRSTRLEN);
    std::stringstream ss;
    ss.clear();
    std::string out;
    out.clear();
    inet_ntop(AF_INET, &nlri.prefix, buffer, INET_ADDRSTRLEN);
    out = buffer;
    out += "/" + int_to_string(nlri.prefix_length);
    ss << std::setw(32) << std::setiosflags(std::ios::left) << out;
    return ss.str();
}

std::string next_hopv4_print(const bgp_update &update) {

    char buffer[INET_ADDRSTRLEN];
    memset(&buffer, 0, INET_ADDRSTRLEN);
    std::stringstream ss;
    inet_ntop(AF_INET, &update.ipv4_next_hop, buffer, INET_ADDRSTRLEN);
    ss << std::setw(32) << std::setiosflags(std::ios::left) << buffer;
    return ss.str();
}
/** BGP::show_bgp_adj_rib_in - Writes adj-rib-in entries to pty
 */
std::string BGP::show_bgp_adj_rib_in(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    s_s << "\nBGP table version is " << bgp->table_version() << ", local router ID is " <<
            (bgp->bgp_id_ip() ? bgp->bgp_id_ip() : "NOT_SET") << std::endl;

    s_s << "Status codes: " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::suppressed] << " suppressed, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::damped] << " damped, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::history] << " history, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::valid] << " valid, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::best] << " best, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::internal] << " - internal, \n\t" <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::rib_failure] << " RIB-failure, " <<
            bgp_rib_code_text[(uint8_t) bgp_rib_codes::stale] << " Stale\n";

    s_s << "Origin codes: " <<
            bgp_origin_code_text[(uint8_t) bgp_origin_type::igp] << " - IGP, " <<
            bgp_origin_code_text[(uint8_t) bgp_origin_type::egp] << " - EGP, " <<
            bgp_origin_code_text[(uint8_t) bgp_origin_type::incomplete] << " - incomplete\n\n";

    s_s << "RIB total size\t\t= " <<
            (sizeof (bgp->get_adj_rib_in()) + sizeof (bgp_update) * bgp->get_adj_rib_in().size())
            << "(bytes allocated) ";

    s_s << "\nRIB max size\t\t= " << bgp->get_adj_rib_in().max_size() << "(entries)";
    s_s << "\nRIB updates in queue\t\t=" << bgp->adj_rib_in_q.size();
    s_s << "\nRIB withdraws in queue\t\t=" << bgp->adj_rib_in_wdraw_q.size();

    s_s << "\n\n   " << std::setw(32) << std::setiosflags(std::ios::left) <<
            "Network" << std::setw(32) << std::setiosflags(std::ios::left)
            << "Next Hop"
            << std::setw(10) << std::setiosflags(std::ios::right) << "Metric"
            << std::setw(7) << std::setiosflags(std::ios::left) << "LocPrf"
            << std::setw(7) << std::setiosflags(std::ios::left) << "Weight"
            << std::setw(7) << std::setiosflags(std::ios::left) << "Path\n";

    bgp->lock_adj_rib_in();

    for (const auto &entry : bgp->get_adj_rib_in()) {

        s_s << "   ";

        switch (entry.second.addr_family) {
            case AF_INET:
                s_s << prefixv4_wlen_print(entry.second.nlri);

                break;
#ifdef USE_AF_INET6
            case AF_INET6:
                break;
#endif
        }

        switch (entry.second.addr_family) {
            case AF_INET:
                s_s << std::setw(32) <<
                        std::setiosflags(std::ios::left) <<
                        next_hopv4_print(entry.second);
                break;
#ifdef USE_AF_INET6
            case AF_INET6:
                break;
#endif
            default:
                break;
        }

        s_s << std::setw(10) << std::setiosflags(std::ios::left)
                << entry.second.med
                << std::setw(7) << std::setiosflags(std::ios::right)
                << entry.second.local_pref
                << std::setw(7) << std::setiosflags(std::ios::right)
                << 0 << std::setw(2);

        s_s << std::setiosflags(std::ios::left) <<
                ((entry.second.as_path->seg_type == bgp_as_path_type::as_set)
                ? "{" : "");

        if (entry.second.as4_path != nullptr) {
            s_s << "AS4_PATH(";
            for (uint8_t i(0); i < entry.second.as4_path->length; ++i) {
                s_s <<
                        std::setiosflags(std::ios::left) <<
                        entry.second.as4_path->seg_value[i];

                if ((i + 1) < entry.second.as4_path->length)s_s << ' ';
            }
            s_s << ")";
        } else {
            for (uint8_t i(0); i < entry.second.as_path->length; ++i) {
                s_s <<
                        std::setiosflags(std::ios::left) <<
                        entry.second.as_path->seg_value[i];

                if ((i + 1) < entry.second.as_path->length)s_s << ' ';
            }
        }

        s_s << std::setiosflags(std::ios::left) <<
                ((entry.second.as_path->seg_type == bgp_as_path_type::as_set)
                ? "}" : "");


        s_s << ' ' << bgp_origin_code_text[entry.second.origin];
        s_s << std::endl;
    }

    bgp->unlock_adj_rib_in();

    s_s << "\n\n";
    tokens.clear();
    return s_s.str();
}
/** BGP::show_bgp_adj_rib_wdrawn - Writes withdrawn-rib entries to pty
 */
std::string BGP::show_bgp_adj_rib_wdrawn(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();
    s_s << "% not implemented";
    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_peer_rcv_buffer_size(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    int tcp_buffer_size = atoi(tokens[3].c_str());
    int bgp_buffer_size = atoi(tokens[4].c_str());

    peer->get_ref_tcp_rx_socket_size() = tcp_buffer_size;
    peer->get_ref_bgp_rx_buffer_size() = bgp_buffer_size;

    if (peer->connected()) {
        if (!peer->set_tcp_bgp_rx_buffer_size(tcp_buffer_size, bgp_buffer_size))
            s_s << "% Error setting receive buffer size to tcp=" << tokens[3]
                << " and bgp=" << tokens[4] << std::endl;
        else syslog(LOG_DEBUG, "set rx buffers for peer %s: tcp=%i bgp=%i",
                peer->p_ip, tcp_buffer_size, bgp_buffer_size);
    } else {
        peer->set_tcp_rx_buffer_size(tcp_buffer_size);
        s_s << "% Peer " << peer->p_ip << " state is "
                << peer_state[(uint8_t) peer->state]
                << ": will resize rx-buffers on Established\n";
    }

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_peer_snd_buffer_size(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    int tcp_buffer_size = atoi(tokens[3].c_str());
    int bgp_buffer_size = atoi(tokens[4].c_str());

    peer->get_ref_tcp_tx_socket_size() = tcp_buffer_size;
    peer->get_ref_bgp_tx_buffer_size() = bgp_buffer_size;

    if (peer->connected()) {
        if (!peer->set_tcp_bgp_tx_buffer_size(tcp_buffer_size, bgp_buffer_size))
            s_s << "% Error setting send buffer size to tcp=" << tokens[3]
                << " and bgp=" << tokens[4] << std::endl;
        else syslog(LOG_DEBUG, "set tx buffers for peer %s: tcp=%i bgp=%i",
                peer->p_ip, tcp_buffer_size, bgp_buffer_size);
    } else {
        peer->set_tcp_tx_buffer_size(tcp_buffer_size);
        s_s << "% Peer " << peer->p_ip << " state is "
                << peer_state[(uint8_t) peer->state]
                << ": will resize tx-buffers on Established\n";
    }

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute(BGP *bgp,
        std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::proto_bgp_attribute_bgp_id_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    std::string value = tokens[5];

    s_s << peer->set_translation(bgp_path_attrib::bgp_id, peer->bgp_id, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute_origin_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    std::string temp = "";
    uint32_t tmp = 0;

    s_s << peer->set_translation(bgp_path_attrib::origin, tmp, temp);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_peer_timers(BGP *bgp,
        std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::proto_bgp_peer_attribute(BGP *bgp,
        std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::proto_bgp_attribute_aspath_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    uint32_t match = string_to_uint32_t(tokens[5]);
    std::string value = tokens[6];

    s_s << peer->set_translation(bgp_path_attrib::as_path, match, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute_nexthop_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::proto_bgp_attribute_v4nexthop_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    uint32_t match(0);

    if (inet_pton(AF_INET, tokens[6].c_str(), &match) < 0) {
        s_s << "% Invalid next-hop " << tokens[6] << std::endl;
        s_s.clear();
        return s_s.str();
    }

    std::string value = tokens[7];

    s_s << peer->set_translation(bgp_path_attrib::next_hop, match, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute_med_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    uint32_t match = string_to_uint32_t(tokens[5]);
    std::string value = tokens[6];

    s_s << peer->set_translation(bgp_path_attrib::multi_exit_disc, match, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute_localpref_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    uint32_t match = string_to_uint32_t(tokens[5]);
    std::string value = tokens[6];

    s_s << peer->set_translation(bgp_path_attrib::local_pref, match, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::proto_bgp_attribute_atomicaggr_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    return "";
}

std::string BGP::kill_all_signal(BGP *bgp, std::vector<std::string> &tokens) {

    bgp->kill_all();

    tokens.clear();
    return std::string("% Sent: this->kill_all()");
}

std::string BGP::proto_bgp_attribute_community_translate_value(BGP *bgp,
        std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        s_s.clear();
        return s_s.str();
    }

    uint32_t match = string_to_uint32_t(tokens[5]);
    std::string value = tokens[6];

    s_s << peer->set_translation(bgp_path_attrib::community, match, value);

    tokens.clear();
    return s_s.str();
}

std::string BGP::set_peer_logging(BGP *bgp, std::vector<std::string> &tokens) {
    std::stringstream s_s;
    s_s.clear();

    bgp_peer *peer = bgp->get_peer_by_str_ip(tokens[1]);

    if (peer == nullptr) {
        s_s << "% Peer " << tokens[1] << " does not exist\n";
        tokens.clear();
        return s_s.str();
    }

    std::string neighbor = tokens[1];
    std::string logging_host = tokens[4];

    if (!string_to_ip(logging_host)) {
        s_s << "% " << logging_host << " is not a valid host address\n";
        tokens.clear();
        return s_s.str();
    }

    std::string logging_port = tokens[5];

    uint32_t bgp_id = peer->bgp_id;
    uint16_t port = string_to_uint16_t(logging_port);

    port = htons(port);
    bgp->add_logging_host(bgp_id, logging_host, port);

    tokens.clear();
    return s_s.str();
}