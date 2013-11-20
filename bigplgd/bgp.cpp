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

#include <algorithm>
#include <utility>
#include <syslog.h>

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
#include <iostream>
#endif

#include "misc.h"
#include "bgp.h"
#include "bgp_thread-mgr.h"
#include "bgp_peer.h"
#include "bgp_msg.h"
#include "bgp_msg_text.h"
#include "bgp_timer.h"
#include "bgp_database.h"

using namespace command_parser;
const BGP *parent_bgp_instance;

/** Initialize default values.
 */
BGP::BGP() : cmd_parser(), thread_mgr(), tcp_server(), bgp_database(),
dumper_on_off(false), stop_and_exit_instance(false), bgp_id(0),
hold_time(180), AS(0), port_(179), version(4), fsm_state_wait__(10),
table_version_(0) {
    db_open("/tmp/wd_db");
    parent_bgp_instance = this;
    memset(b_ip, 0, sizeof (b_ip));
}

/** Cleanup on exit.
 */
BGP::~BGP() {
    this->stop_and_exit_instance = true;

    adj_rib_in.clear();

    extern int lock_file;
    ::close(lock_file);
    closelog();
    remove("/tmp/bgp_lock");
}

/** Initialize command parser.
 *
 * main thread.
 * inits command struct.
 * starts tcp server.
 * processes rib-queues.
 *
 * as - 2 byte autonomous system number.
 */
void BGP::init_BGP(uint16_t &as) {
    syslog(LOG_DEBUG, "BGP::InitBGP() AS =  %i", as);

    this->AS = as;
    CmdInit();

#ifndef BGP_UNIT_TEST

    // listen only on localhost for CLI
    while (!this->listen_for_cli("127.0.0.1", "666"));

    // listen on all interfaces for api-calls
    while (!this->listen_for_api("0.0.0.0", "667"));

    // should have a stop condition here
    while (!this->stop_and_exit()) {

        this->proc_queue_wait();

        // check peer queues
        for (auto peer : this->thread_q) {
            this->proc_in_q(peer.get());
        }

        proc_adj_rib_in_queue();
        proc_adj_rib_wdraw_queue();

    }
#endif

}

/** Initiate peer FSM.
 *
 * new thread created from CLI.
 * clear counters.
 * init timers.
 * enter established loop.
 *
 * bgp - pointer to BGP instance.
 * peer - pointer to peer instance.
 */
int BGP::init_peer(BGP *bgp, bgp_peer *peer) {
    syslog(LOG_DEBUG,
            "BGP::init_peer() called for AS: %i, managing peer %s remote-as %i",
            bgp->ASN(), peer->p_ip, peer->AS);

#ifdef BGP_UNIT_TEST
    return 0;
#else

    while (peer->state != bgp_peer_state::deleted && (!bgp->stop_and_exit())) {
        bgp->init_peer_states(peer);
        bgp->peer_established(peer);
    }//while (peer->state != DELETED)

    // peer was de-configured, delete everything about this peer
    return bgp->delete_peer(peer);
#endif
}

/** Initial state of peer.
 *
 * attempts to tcp connect, will retry on failure and
 * remain in bgp_peer_state::idle until success.
 * on connect, resize buffers.
 * on success, send our open message to peer.
 *
 * bgp - pointer to BGP instance.
 * peer - pointer to peer instance.
 */
void BGP::init_peer_states(bgp_peer *peer) {

    peer->state = bgp_peer_state::idle;
    syslog(LOG_DEBUG,
            "peer %s state = %s",
            peer->p_ip, peer_state[(uint8_t) peer->state]);

    while (peer->state == bgp_peer_state::idle) {
        peer->state = bgp_peer_state::active;
        syslog(LOG_DEBUG,
                "peer %s state = %s",
                peer->p_ip, peer_state[(uint8_t) peer->state]);

        if (!peer->connect(peer->p_ip, int_to_string(this->port()).c_str())) {
            syslog(LOG_DEBUG, "peer %s, unable to connect", peer->p_ip);

            peer->disconnect();

            peer->state = bgp_peer_state::idle;
            syslog(LOG_DEBUG,
                    "peer %s state = %s",
                    peer->p_ip, peer_state[(uint8_t) peer->state]);

            this->state_wait();
            continue;
        }

        peer->state = bgp_peer_state::connect;
        syslog(LOG_DEBUG,
                "peer %s state = %s",
                peer->p_ip, peer_state[(uint8_t) peer->state]);

        if (!peer->set_bgp_rx_buffer_size(peer->get_ref_bgp_rx_buffer_size())) {
            syslog(LOG_DEBUG, "unable to resize BGP rx buffer");
            peer->disconnect();
            peer->state = bgp_peer_state::idle;
        }

        if (!peer->set_bgp_tx_buffer_size(peer->get_ref_bgp_tx_buffer_size())) {
            syslog(LOG_DEBUG, "unable to resize BGP tx buffer");
            peer->disconnect();
            peer->state = bgp_peer_state::idle;
        }

        if (!peer->set_tcp_rx_buffer_size(peer->get_ref_tcp_rx_socket_size())) {
            syslog(LOG_DEBUG, "unable to resize TCP rx buffer");
            peer->disconnect();
            peer->state = bgp_peer_state::idle;
        }

        if (!peer->set_tcp_tx_buffer_size(peer->get_ref_tcp_tx_socket_size())) {
            syslog(LOG_DEBUG, "unable to resize TCP tx buffer");
            peer->disconnect();
            peer->state = bgp_peer_state::idle;
        }
    }// idle

    // send open message to peer.
    this->send_open(peer);
}

/** Peer has passed checks and is up.
 *
 * read messages from peer, and determine types.
 * push updates to peer update queue.
 *
 * bgp - pointer to BGP instance.
 * peer - pointer to peer instance.
 */
void BGP::peer_established(bgp_peer *peer) {

#ifdef BGP_GDB_DEBUG_TEST_PACKET
    //#include "test_packets.h"
#endif

#ifndef BGP_GDB_DEBUG_TEST_PACKET
    // num of bytes read
    size_t bytes = 0;

    // header info
    bgp_header *header = nullptr;
    queue_data *q = nullptr;

    // message buffer
    unsigned char *buffer = nullptr;

    // header buffer should be 19, but I'm padding with trailing 0's.
    unsigned char head_buffer[24];


    //while conditions permit read/write
    while (peer->connected() && peer->state != bgp_peer_state::idle &&
            (!this->stop_and_exit())) {

        // allocate new header
        header = new (std::nothrow) bgp_header;

        if (header == nullptr) {
            syslog(LOG_CRIT,
                    "malloc of packet header from FSM failed, peer=%s",
                    peer->p_ip);

            // drop back to bgp_peer_state::idle and try again
            peer->state = bgp_peer_state::idle;
            return;
        }

        // zero header info and buffer
        memset(&head_buffer, 0, sizeof (unsigned char [24]));
        memset(header, 0, sizeof (bgp_header));

        // attempt to read BGP header
        if ((bytes = peer->read(&head_buffer, 1, 19)) == 19) {

            // time stamp last read
            peer->last_IO.last_read = time(NULL);
            // extract header marker
            memcpy(header->marker, &head_buffer, 16);

            // little endian header length
            header->len = (head_buffer[16] << 8) + head_buffer[17];

            // alloc message buffer length of header->len
            buffer = new (std::nothrow)
                    unsigned char [header->len];

            if (buffer == nullptr) {
                syslog(LOG_CRIT,
                        "malloc of packet buffer from FSM failed, peer=%s",
                        peer->p_ip);

                // assuming alloc header succeeded
                delete header;

                // alloc buffer failed drop to bgp_peer_state::idle
                peer->state = bgp_peer_state::idle;
                return;
            }

            // message type code
            header->type = (bgp_msg_type) head_buffer[18];

            // read up to header->len - header size(19)
            if ((bytes = peer->read(buffer, 1, header->len - 19)) ==
                    (size_t) (header->len - 19)) {

                switch (header->type) {
                    case bgp_msg_type::open:
                        syslog(LOG_DEBUG,
                                "received open message from %s",
                                peer->p_ip);

                        peer->opens.rx++;
                        peer->opens.last_rx = time(NULL);
                        this->decode_open(peer, buffer, header->len - 19);

                        break;
                    case bgp_msg_type::keepalive:
                        syslog(LOG_DEBUG,
                                "received keepalive from %s",
                                peer->p_ip);

                        peer->keepaliv.rx++;
                        peer->keepaliv.last_rx = time(NULL);

                        /* if peer is bgp_peer_state::openconfirm then this KEEPALIVE
                         * is meant to ESTABLISH the BGP session */
                        if (peer->state == bgp_peer_state::openconfirm) {
                            peer->state = bgp_peer_state::established;
                            syslog(LOG_DEBUG,
                                    "peer %s state = %s",
                                    peer->p_ip, peer_state[(uint8_t) peer->state]);
                            this->BGP_5_ADJCHANGE(*peer, "Up");
                            peer->up_since = time(NULL);
                        }

                        // reactive KEEPALIVE
                        this->send_packet(peer, 0, 0, bgp_msg_type::keepalive);

                        break;
                    case bgp_msg_type::notification:
                        syslog(LOG_DEBUG,
                                "received notification from %s",
                                peer->p_ip);

                        peer->notify.rx++;
                        peer->notify.last_rx = time(NULL);
                        this->decode_notification(peer, buffer, header->len - 19);

                        break;
                    case bgp_msg_type::update:

                        // handle update by pushing to peers InQ
                                peer->update.rx++;
                        peer->update.last_rx = time(NULL);

                        q = new (std::nothrow) queue_data;

                        if (q != nullptr) {
                            q->length = (header->len - 19);

                            memcpy(&q->packet, buffer,
                                    (q->length));

                            peer->lock_in_queue();
                            peer->in_queue.push(*q);
                            peer->in_q++;
                            peer->unlock_in_queue();

                            delete q;
                            q = nullptr;
                        }

#elif defined USE_ESTABLISHED_TO_PUSH_QUEUE
    peer->lock_in_queue();
    peer->in_queue.push(test_packet2);
    peer->in_q++;
    peer->unlock_in_queue();
#endif
#ifndef BGP_GDB_DEBUG_TEST_PACKET


                        break;
                    case bgp_msg_type::routerefresh:
                        syslog(LOG_DEBUG,
                                "received router refresh from %s",
                                peer->p_ip);

                        peer->routeref.rx++;
                        peer->routeref.last_rx = time(NULL);

                        break;
                    default:
                        break;
                }
            }
            if (buffer != nullptr)delete [] buffer;
        }
        if (header != nullptr)delete header;
    }
#endif

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
#endif
#ifndef BGP_GDB_DEBUG_TEST_PACKET
    // flush ALL learned routes from peer they are no longer valid
    while (this->flush_peer(peer->bgp_id))--peer->prefixes.current.rx;
#endif
#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast < duration<double >> (t2 - t1);
    std::cout << "flush_peer finished in: " << time_span.count() << "(secs)" << std::endl;
#endif

#ifndef BGP_GDB_DEBUG_TEST_PACKET
    // if peer is established then we've reached EOF of the socket
    if (peer->state == bgp_peer_state::established && !peer->connected())
        this->BGP_5_ADJCHANGE(*peer, "Down Peer closed the session");

    peer->disconnect();
    peer->up_since = 0;

    // empty peer queues
    peer->lock_in_queue();
    while (!peer->in_queue.empty()) peer->in_queue.pop();
    peer->unlock_in_queue();

    // XXX not implemented, also needs lock
    while (!peer->out_queue.empty()) peer->out_queue.pop();
#endif

}

/** Checks hold_time and sends keepalives.
 *
 * peer - peer instance.
 */
bool BGP::hold_timer_expired(bgp_peer *peer) {

    if (peer->state != bgp_peer_state::established) return true;

    time_t now = time(NULL);

    /* check the last time we've received a KEEP_ALIVE
     * if "now" > than hold-time, expire session */
    if ((now - peer->last_IO.last_read) >= peer->hold_time) {
        syslog(LOG_DEBUG,
                "peer %s, hold timer ticks every %i seconds",
                peer->p_ip, peer->hold_time);
        this->notification(peer, bgp_error_code::hold_time_expired, bgp_error_subcode::zero);
        this->BGP_5_ADJCHANGE(*peer, "Down Holdtime expired");
        peer->state = bgp_peer_state::idle;
        peer->disconnect();
        return true;
    }

    if ((now - peer->keepaliv.last_tx) >= peer->keep_alive) {
        this->send_packet(peer, 0, 0, bgp_msg_type::keepalive);
    }

    return false;
}

/** Processes InQ.
 */
void BGP::proc_adj_rib_in_queue(void) {

    if (this->adj_rib_in_q.empty()) return;

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
#endif

    try {
        while (!this->adj_rib_in_q.empty()) {

            this->peer_entry(this->adj_rib_in_q.front());

            //will block proc_in_q
            this->lock_adj_rib_in_queue();
            this->adj_rib_in_q.pop();
            this->unlock_adj_rib_in_queue();

            this->table_version()++;

        }

    } catch (...) {

        this->unlock_adj_rib_in();
        this->unlock_adj_rib_in_queue();

    }

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast < duration<double >> (t2 - t1);
    std::cout << "proc_adj_rib_in_queue finished in: "
            << time_span.count() << "(secs)" << std::endl;
#endif

}

/** Process withdrawQ.
 */
void BGP::proc_adj_rib_wdraw_queue(void) {

    if (this->get_adj_rib_wdraw_q().empty()) return;

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
#endif

    try {
        while (!this->adj_rib_in_wdraw_q.empty()) {

            this->withdraw(this->adj_rib_in_wdraw_q.front());

            //will block proc_in_q
            this->lock_adj_rib_in_wdraw_queue();
            this->get_adj_rib_wdraw_q().pop();
            this->unlock_adj_rib_in_wdraw_queue();

            this->table_version()++;

        }

    } catch (...) {

        this->unlock_adj_rib_in();
        this->unlock_adj_rib_in_wdraw_queue();

    }

#ifdef BGP_GDB_DEBUG_RIB_PROFILE
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast < duration<double >> (t2 - t1);
    std::cout << "proc_adj_rib_wdraw_queue finished in: "
            << time_span.count() << "(secs)" << std::endl;
#endif

}

/** key pair [prefix,bgp_id] bgp_update to adj-rib-in.
 *
 * entries are stored and sorted
 * in key pair [prefix,bgp_id]. if prefix == prefix,
 * bgp_id breaks tie.
 *
 * current scheme convergence @ ~78 seconds
 * for a full internet table.
 *
 * overwriting key pair prevents duplicates and
 * signals an implicit withdraw.
 *
 * update - update pulled off the queue to be inserted in to RIB.
 */
int BGP::peer_entry(const bgp_update &update) {

    //if logging, then dump to log
    if (this->dumper_on_off)
        this->dumper.translate_and_dump_update(this, update, nlri_parse::update);

    bgp_adj_rib::iterator it;

    //lock rib
    this->lock_adj_rib_in();

#ifndef BGP_GDB_DEBUG_TEST_PACKET

    // check for implicit withdraw
    it = adj_rib_in.find(std::make_pair(update.nlri.prefix, update.nlri.bgp_id));

    bgp_peer *peer = this->get_peer_by_uint32_t_bgp_id(update.nlri.bgp_id);

    if (peer == nullptr) {
        this->unlock_adj_rib_in();
        return 0;
    }

    peer->lock();

    if (it != adj_rib_in.end()) {
        ++peer->implicit_withdraw.rx;
        db_insert(it->second);
    } else {
        ++peer->prefixes.current.rx;
    }

    ++peer->prefixes.rx;

    peer->unlock();

#endif
    //make unique entry or over write existing
    adj_rib_in[std::make_pair(update.nlri.prefix, update.nlri.bgp_id)] = update;

    //unlock rib
    this->unlock_adj_rib_in();

    return 0;
}

/** Find prefix and remove from adj-rib-in.
 *
 * nlri_withdraw - update pulled off queue with prefix info to delete.
 */
int BGP::withdraw(const bgp_update &nlri_withdraw) {

    bgp_adj_rib::iterator it;

    //lock rib
    this->lock_adj_rib_in();

    it = adj_rib_in.find(std::make_pair(nlri_withdraw.nlri.prefix,
            nlri_withdraw.nlri.bgp_id));

    if (it != adj_rib_in.end()) {

        //if logging, then dump to log
        if (this->dumper_on_off) {
            this->dumper.translate_and_dump_update(
                    this, it->second, nlri_parse::withdraw
                    );
        }

        bgp_peer *peer = this->get_peer_by_uint32_t_bgp_id(
                nlri_withdraw.nlri.bgp_id
                );

        peer->lock();
        ++peer->explicit_withdraw.rx;
        --peer->prefixes.current.rx;
        peer->unlock();

        adj_rib_in.erase(it);
    }

    //unlock rib
    this->unlock_adj_rib_in();

    return 0;
}

/** Delete entries from peer due to clear/down session.
 *
 * bgp_id - BGP ID of peer.
 */
int BGP::flush_peer(const uint32_t &bgp_id) {

    bgp_adj_rib::iterator it;
    int rc(0);

    this->lock_adj_rib_in();

    for (it = adj_rib_in.begin(); it != adj_rib_in.end(); ++it) {
        if (it->second.nlri.bgp_id == bgp_id) {
            adj_rib_in.erase(it);
            rc = 1;
            break;
        }
    }

    this->unlock_adj_rib_in();

    return rc;
}