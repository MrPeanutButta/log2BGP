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

#ifndef _BGP_H_
#define _BGP_H_

#include <cstring>
#include <list>
#include <map>
#include <utility>
#include <arpa/inet.h>
#include "bgp_thread-mgr.h"
#include "bgp_peer.h"
#include "bgp_msg.h"
#include "cli/bgp_cmdparser.h"
#include "bgp_adj_rib_wdrawn.h"
#include "tcp_server.h"
#include "bgp_syslog.h"
#include "bgp_adj_rib.h"
#include "bgp_database.h"

/** Performs all management of peer fsm thread and cli commands.
 *
 * Main thread processes rib-in-queue.
 * TCP server thread serves as CLI parser.
 */
class BGP : command_parser::cmd_parser, thread_mgr, tcp_server, bgp_database {
public:
    BGP();
    ~BGP();

    bool hold_timer_expired(bgp_peer *);
    static int init_peer(BGP *, bgp_peer *);

    //queue checking on an interval
    void proc_adj_rib_in_queue(void);
    void proc_adj_rib_wdraw_queue(void);

    void init_BGP(uint16_t &);

    // peer specific actions
    int send_open(bgp_peer *);
    void decode_open(bgp_peer *, unsigned char *, unsigned short);
    void notification(bgp_peer *, bgp_error_code, bgp_error_subcode);
    void notification(bgp_peer *, bgp_error_code, bgp_open_error_subcode);
    void decode_notification(bgp_peer *, unsigned char *, unsigned short);

    void init_peer_states(bgp_peer *);
    void peer_established(bgp_peer *);
    int proc_in_q(bgp_peer *);
    int send_packet(bgp_peer *, unsigned char *, unsigned, bgp_msg_type);

    // API specific

    /* returns all current info on prefix
     * /bgp/ipv4/:prefix
     * /bgp/ipv4/:prefix/history
     *
     * api-get prefix x.x.x.x
     * api-get prefix x.x.x.x history
     */
    static std::string api_get_prefix(BGP *, std::vector<std::string> &);

    /* returns all current prefixes from origin ASN
     * /bgp/ipv4/from-asn/:ASN/:length
     *
     * api-get all-from-asn :asn :length
     */
    static std::string api_get_all_origin_as(BGP *, std::vector<std::string> &);

    /* returns all non rfc 1918 from ASN
     * '/bgp/ipv4/all/public/from-asn/:asn/:length'
     *
     * api-get public-from-asn :asn :length
     */
    static std::string api_get_public_origin_as(BGP *, std::vector<std::string> &);

    /* returns all rfc 1918 from ASN
     * '/bgp/ipv4/all/private/from-asn/:asn/:length'
     *
     * api-get private-from-asn :asn :length
     */
    static std::string api_get_private_origin_as(BGP *, std::vector<std::string> &);

    /* returns all prefixes with noexport and noadvertise community
     * values set
     * /bgp/ipv4/noexport_noadvertise */
    static std::string api_get_all_noexport_noadvertise(BGP *, std::vector<std::string> &);

    /* returns all :prefix info for a time range in unix epoch
     * /bgp/ipv4/:prefix/:time_start/:time_end */
    static std::string api_get_prefix_entries_timerange(BGP *, std::vector<std::string> &);

    // CLI specific

    inline std::string tcp_execute_line(const char *command_line) {
        return this->ExecuteLine(this, command_line);
    };

    static std::string proto_bgp_neighbor(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_peer_timers(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_peer_attribute(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_bgp_id_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_aspath_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_nexthop_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_v4nexthop_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_med_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_localpref_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_atomicaggr_translate_value(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_community_translate_value(BGP *, std::vector<std::string> &);

    static std::string set_bgp_id(BGP *, std::vector<std::string> &);
    static std::string Show(BGP *, std::vector<std::string> &);
    static std::string show_bgp_neighbors(BGP *, std::vector<std::string> &);
    static std::string show_bgp_adj_rib_in(BGP *, std::vector<std::string> &);
    static std::string show_bgp_adj_rib_wdrawn(BGP *, std::vector<std::string> &);
    static std::string show_bgp_prefix(BGP *, std::vector<std::string> &);
    static std::string show_bgp_prefix_w_len(BGP *, std::vector<std::string> &);
    static std::string write_configs(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_peer_rcv_buffer_size(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_peer_snd_buffer_size(BGP *, std::vector<std::string> &);
    static std::string set_peer_logging(BGP *, std::vector<std::string> &);

    static std::string proto_bgp_attribute(BGP *, std::vector<std::string> &);
    static std::string proto_bgp_attribute_origin_translate_value(BGP *, std::vector<std::string> &);
    static std::string kill_all_signal(BGP*, std::vector<std::string> &);
    static std::string proto_bgp_toggle_update_dump(BGP*, std::vector<std::string> &);

    std::string translate_bgp_id(const uint32_t);
    std::string translate_origin(const uint32_t, const uint8_t);
    std::string translate_as_path(const uint32_t, const uint32_t);
    std::string translate_ipv4_next_hop(const uint32_t, const uint32_t);
    std::string translate_med(const uint32_t, const uint32_t);
    std::string translate_local_pref(const uint32_t, const uint32_t);
    std::string translate_attomic_aggr(const uint32_t, const uint8_t);
    std::string translate_aggregator(const uint32_t, const uint32_t);
    std::string translate_community_value(const uint32_t, const uint16_t);

    // sets bgp_id from CLI

    inline void set_id(const uint32_t &n_ip) {
        this->lock();

        try {
            in_addr nn_ip;
            memcpy(&this->bgp_id, &n_ip, sizeof (uint32_t));
            nn_ip.s_addr = n_ip;

            strcpy(this->b_ip, inet_ntoa(nn_ip));
        } catch (...) {
        }

        this->unlock();
    }

    inline uint16_t ASN(void) {
        return this->AS;
    }

    inline uint8_t port(void) {
        return this->port_;
    }

    inline bgp_peer *peer_at(const int &index) {
        return thread_base_peer_at(index);
    }

    inline char *bgp_id_ip(void) {
        return this->b_ip;
    }

    inline uint32_t BGP_ID(void) {
        return this->bgp_id;
    }

    inline uint8_t fsm_state_wait(void) {
        return this->fsm_state_wait__;
    }

    inline int &table_version(void) {
        return this->table_version_;
    }

    inline int push_adj_rib_in_q(const bgp_update *update) {
        this->lock_adj_rib_in_queue();

        try {
            this->adj_rib_in_q.push(*update);
        } catch (...) {
        }

        this->unlock_adj_rib_in_queue();

        return 0;
    }

    inline int push_adj_rib_in_wdraw_q(const bgp_update *update) {
        this->lock_adj_rib_in_wdraw_queue();

        try {
            this->adj_rib_in_wdraw_q.push(*update);
        } catch (...) {
        }

        this->unlock_adj_rib_in_wdraw_queue();

        return 0;
    }

    inline bgp_adj_rib &get_adj_rib_in(void) {
        return this->adj_rib_in;
    }

    inline std::queue<bgp_update> &get_adj_rib_in_q(void) {
        return this->adj_rib_in_q;
    }

    inline std::queue<bgp_update> &get_adj_rib_wdraw_q(void) {
        return this->adj_rib_in_wdraw_q;
    }

    inline void state_wait(void) {
        nano_sleep();
    }

    inline bool &stop_and_exit(void) {
        return this->stop_and_exit_instance;
    }

    inline void kill_all(void) {
        this->stop_and_exit_instance = true;
    }

    inline void lock(void) {
        while (!this->bgp_mutex.try_lock()) {
            nano_sleep();
        }
    }

    inline void unlock(void) {
        this->bgp_mutex.unlock();
    }

    inline void lock_adj_rib_in(void) {
        while (!this->adj_rib_in_mutex.try_lock()) {
            nano_sleep();
        }
    }

    inline void unlock_adj_rib_in(void) {
        this->adj_rib_in_mutex.unlock();
    }

    inline void lock_adj_rib_in_queue(void) {
        while (!this->adj_rib_in_q_mutex.try_lock()) {
            nano_sleep();
        }
    }

    inline void unlock_adj_rib_in_queue(void) {
        this->adj_rib_in_q_mutex.unlock();
    }

    inline void lock_adj_rib_in_wdraw_queue(void) {
        while (!this->adj_rib_in_wdraw_q_mutex.try_lock()) {
            nano_sleep();
        }
    }

    inline void unlock_adj_rib_in_wdraw_queue(void) {
        this->adj_rib_in_wdraw_q_mutex.unlock();
    }

    inline void dumper_on(void) {
        this->dumper_on_off = true;
    }

    inline void dumper_off(void) {
        this->dumper_on_off = false;
    }

    inline bool dump_updates(void) {
        return this->dumper_on_off;
    }

    /** Syslog reference
     *  syslog() - macros with list of severity Levels:
     *0 -       Emergency: system is unusable
     *1 -       Alert: action must be taken immediately
     *2 -       Critical: critical conditions
     *3 -       Error: error conditions
     *4 -       Warning: warning conditions
     *5 -       Notice: normal but significant condition
     *6 -       Informational: informational messages
     *7 -       Debug: debug-level messages
     */
    void BGP_5_ADJCHANGE(bgp_peer &peer, std::string up_down) {
        closelog();
        openlog(c_BGP_5_ADJCHANGE, LOG_CONS, LOG_LOCAL7);
        syslog(LOG_NOTICE, "neighbor %s %s", peer.p_ip, up_down.c_str());
        closelog();
        openlog("BGP", LOG_CONS, LOG_LOCAL7);
    }

    void BGP_3_NOTIFICATION(std::string err_code, std::string err_sub_code) {
        closelog();
        openlog(c_BGP_3_NOTIFICATION, LOG_CONS, LOG_LOCAL7);
        syslog(LOG_ERR, "%s%s", err_code.c_str(), err_sub_code.c_str());
        closelog();
        openlog("BGP", LOG_CONS, LOG_LOCAL7);
    }

    // formats a RIB entry.
    static std::string display_entry(BGP *, bgp_update *);

    // formats a RIB entry to JSON
    std::string jsonify_entry(bgp_update *);

    void add_logging_host(const uint32_t& ipv4,
            const std::string& dest_host_ipv4, const uint16_t& port) {
        dumper.add_dest_host(ipv4, dest_host_ipv4, port);
    }

    udp_destinations &get_syslog_config(void) {
        return dumper.get_udp_destinations();
    }

private:

    void nano_sleep(int nano_secs = 20) {
        std::this_thread::sleep_for(
                std::chrono::nanoseconds(nano_secs)
                );
    }

    // adds/assigns by typedef prefix_entry
    int peer_entry(const bgp_update &);

    // removes prefix matched by typedef prefix_entry
    int withdraw(const bgp_update &);

    /* removes single prefix and returns 1.
     * returns 0 when no entries found */
    int flush_peer(const uint32_t &);

    // extract prefix and length
    void parse_nlri(bgp_peer &, bgp_update *, int &,
            const unsigned char *, nlri_parse);
    // extract AS_PATH
    void parse_as_path(update_data &, bgp_update *,
            const unsigned char *);
    // extract AS4_PATH
    void parse_as4_path(update_data &, bgp_update *,
            const unsigned char *);
    // extract COMMUNITY
    void parse_community(update_data &, bgp_update *,
            const unsigned char *);

    // wait 100 milliseconds before attempting to process RIB queues

    inline void proc_queue_wait(void) {
        std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
                );
    }

    // if true then match and dump to log
    bool dumper_on_off;

    // if true all threads exit
    bool stop_and_exit_instance;

    // my BGP ID
    uint32_t bgp_id; // BGP ID

    // time to wait before dropping me
    uint16_t hold_time;
    uint16_t AS;

    uint8_t port_;

    // BGPv4
    uint8_t version;

    // time to wait before switching states
    uint8_t fsm_state_wait__;

    // printable BGP ID
    char b_ip[16];

    // bgp table version
    int table_version_;

    std::vector<std::string> running_config;

    // update dumper
    bgp_syslog dumper;

    // queued prefixes to be withdrawn
    std::queue<bgp_update> adj_rib_in_wdraw_q;

    // holds prefixes withdrawn stats
    //AdjRibWDrawn adj_rib_withdrawn;

    // queued updates being sent to the RIB
    std::queue<bgp_update> adj_rib_in_q;

    /* ordered map associated by
     * [prefix, bgp_id] key pair */
    bgp_adj_rib adj_rib_in;

    // lock before BGP instance changes
    std::mutex bgp_mutex;

    // lock before adj_rib_in changes
    std::mutex adj_rib_in_mutex;

    // lock before adj_rib_in_q changes
    std::mutex adj_rib_in_q_mutex;

    // lock before adj_rib_in_wdraw_q changes
    std::mutex adj_rib_in_wdraw_q_mutex;

};

#endif // _BGP_H_
