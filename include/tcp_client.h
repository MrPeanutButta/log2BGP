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

#ifndef TCP_CLIENT_H
#define	TCP_CLIENT_H

#include <mutex>
#include <thread>
#include <netdb.h>

class BGP;

class tcp_client {
public:
    tcp_client();
    tcp_client(const tcp_client& orig);
#ifdef TCP_CLIENT_BGPD
    virtual ~tcp_client() = 0;
#endif
#ifdef TCP_CLIENT_CLI
    ~tcp_client();
#endif
    bool connect(const char *, const char *);
    bool connect(void);
    bool connected(void);

    bool set_bgp_rx_buffer_size(const size_t &);
    bool set_tcp_bgp_rx_buffer_size(const size_t &, const size_t &);
    bool set_bgp_tx_buffer_size(const size_t &);
    bool set_tcp_bgp_tx_buffer_size(const size_t &, const size_t &);
    bool set_tcp_tx_buffer_size(const size_t &);
    bool set_tcp_rx_buffer_size(const size_t &);

    size_t read(void *, const size_t &, const size_t &);
    int read_char(void);

    size_t write(void *, const size_t &, const size_t &);
    int write_formated(const char *, const int &, const int &);

    int tx_flush(void);
    int rx_flush(void);

    int get_tcp_rx_buffer_size(void);
    int get_tcp_tx_buffer_size(void);

    void disconnect(void);

    int &get_ref_tcp_rx_socket_size(void) {
        return tcp_rx_socket_buffer_size;
    }

    int &get_ref_tcp_tx_socket_size(void) {
        return tcp_tx_socket_buffer_size;
    }

    int &get_ref_bgp_tx_buffer_size(void) {
        return bgp_tx_buffer_size;
    }

    int &get_ref_bgp_rx_buffer_size(void) {
        return bgp_rx_buffer_size;
    }

    void tcp_client_reset(void);

    void lock_write(void) {
        while (!this->write_mutex.try_lock()) {
            std::this_thread::sleep_for(
                    std::chrono::nanoseconds(5)
                    );
        }
    }

    void unlock_write(void) {
        this->write_mutex.unlock();
    }

    void lock_feof(void) {
        while (!this->feof_mutex.try_lock()) {
            std::this_thread::sleep_for(
                    std::chrono::nanoseconds(5)
                    );
        }
    }

    void unlock_feof(void) {
        this->feof_mutex.unlock();
    }

private:
    bool get_addr_info(const char *, const char *);

    friend class tcp_server;

    addrinfo hints;
    addrinfo *results, *rp;

    int tcp_socket;
    int tcp_rx_socket_buffer_size;
    int tcp_tx_socket_buffer_size;
    int bgp_tx_buffer_size;
    int bgp_rx_buffer_size;

    FILE *tx;
    FILE *rx;

    std::mutex write_mutex;
    std::mutex feof_mutex;
};

#endif	/* TCP_CLIENT_H */

