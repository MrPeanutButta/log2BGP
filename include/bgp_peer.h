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

#ifndef PEER_H
#define	PEER_H

#include <thread>
#include <netdb.h>
#include <syslog.h>
#include <unistd.h>
#include <string>
#include <mutex>
#include <queue>
#include <vector>
#include <chrono>
#include "bgp_msg.h"
#include "bgp_timer.h"
#include "tcp_client.h"

// state flags

enum class bgp_peer_state : uint8_t {
    idle = 0,
    connect,
    active,
    opensent,
    openconfirm,
    established,
    deleted,
    clearing
};

// counter
typedef unsigned int counter;

// generic tx/rx counter

struct byte_tx_rx_current {

    byte_tx_rx_current() :
    tx(0), rx(0) {
    }
    counter tx;
    counter rx;
};

// counter with time stamps

struct bytes_tx_rx {

    bytes_tx_rx() :
    tx(0), rx(0),
    withdrawn(0) {
    }
    counter tx;
    counter rx;

    //added for current # of prefixes
    byte_tx_rx_current current;

    time_t last_rx;
    time_t last_tx;
    counter withdrawn;
};

// connection stats

struct connect_stat {

    connect_stat() :
    estab(0), droped(0),
    last_reset(0) {
    }
    counter estab;
    counter droped;
    counter last_reset;
};

// rx/tx time

struct read_write {
    time_t last_read;
    time_t last_write;
};

// update queue struct

struct queue_data {

    queue_data() :
    length(0) {
        memset(&packet, 0, 4096);
    }

    uint16_t length;
    unsigned char packet[4096];
};

// bgp peer class

class bgp_peer : public tcp_client {
public:

    bgp_peer();
    bgp_peer(const bgp_peer& orig);
    ~bgp_peer();

    uint32_t ipv4; // binary ip for quick eval
    uint32_t bgp_id; // Router ID
    uint32_t update_src_ip; // My update source IP
    uint16_t version; // BGP v4 most likely
    uint16_t hold_time; // Time to wait before dropping neighbor, only need 8 bits, but uint8_t isn't good for stdout
    uint16_t min_hold_time;
    uint16_t keep_alive;
    uint16_t update_freq; // how often to process the InQ, default it 30 seconds
    uint16_t AS; // Remote-AS
    uint32_t opt_param_len; // Optional parameters length in bytes
    uint32_t capabilities_advertisement; // Options
    uint16_t table_version;
    uint16_t in_q;
    uint16_t out_q;
    time_t up_since;

    char p_ip[INET6_ADDRSTRLEN]; // Printable IPv4 or IPv6
    char b_ip[INET_ADDRSTRLEN]; // Printable BGP ID IPv4 notation

    bytes_tx_rx opens; // Message stats
    bytes_tx_rx notify;
    bytes_tx_rx update;
    bytes_tx_rx keepaliv;
    bytes_tx_rx routeref;
    bytes_tx_rx prefixes;
    bytes_tx_rx tx_rx; // Total tx/rx bytes
    connect_stat connect_stats;
    byte_tx_rx_current implicit_withdraw;
    byte_tx_rx_current explicit_withdraw;

    bgp_peer_state state; // Finite state machine state
    std::thread *fsm; // Finite state machine thread

    read_write last_IO; // last read/write action

    // address family
    sa_family_t addr_family;

    //Timer *hold_timer; // hold timer
    Timer *update_timer; // process update queue

    // update queues
    //update_queue in_queue;
    std::queue<queue_data> in_queue;
    std::queue<queue_data> out_queue;

    /// code to text translation features
    std::string set_translation(bgp_path_attrib, uint32_t &, std::string &);
    std::vector<translate_attribs_base> translate_attribs;

    bool doing_alias(void) {
        return is_aliasing;
    }

    inline void lock(void) {
        while (!this->peer_mutex.try_lock()) {
            std::this_thread::sleep_for(
                    std::chrono::nanoseconds(5)
                    );
        }
    }

    inline void unlock(void) {
        this->peer_mutex.unlock();
    }

    inline void lock_in_queue(void) {
        while (!this->in_queue_mutex.try_lock()) {
            std::this_thread::sleep_for(
                    std::chrono::nanoseconds(5)
                    );
        }
    }

    inline void unlock_in_queue(void) {
        this->in_queue_mutex.unlock();
    }

private:

    bool is_aliasing;

    /// mutex
    std::mutex peer_mutex;
    std::mutex in_queue_mutex;
};

#endif	/* PEER_H */

