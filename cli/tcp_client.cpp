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

#include <netdb.h>
#include <syslog.h>
#include <cstring>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include "tcp_client.h"

enum peer_buffer {
    DEFAULT_TCP_BUFFER_SIZE = 14600,
    DEFAULT_BGP_BUFFER_SIZE = 4096,
    MAX_PEER_BUFFER = 131072
};

tcp_client::tcp_client() :
tcp_socket(0),
tcp_rx_socket_buffer_size(DEFAULT_TCP_BUFFER_SIZE),
tcp_tx_socket_buffer_size(DEFAULT_TCP_BUFFER_SIZE),
bgp_tx_buffer_size(DEFAULT_BGP_BUFFER_SIZE),
bgp_rx_buffer_size(DEFAULT_BGP_BUFFER_SIZE) {
    rp = nullptr;
    results = nullptr;
    tx = nullptr;
    rx = nullptr;
    tcp_client_reset();
}

tcp_client::tcp_client(const tcp_client& orig) {
}

tcp_client::~tcp_client() {
    tcp_client_reset();
}

void tcp_client::tcp_client_reset(void) {
    if (tcp_socket)close(tcp_socket);
    memset(&hints, 0, sizeof (addrinfo));
}

/*! tcp_client::get_addr_info - sets server info.
 *
 * sets server info for the server we are connecting to.
 * this must be called before anything.
 */
bool tcp_client::get_addr_info(const char *host, const char *port) {

    hints.ai_family = AF_UNSPEC; // ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    results = 0;

    int s = getaddrinfo(host, port, &hints, &results);

    if (s != 0) {
        syslog(LOG_DEBUG, "get_addr_info: %s", gai_strerror(s));
        return false;
    }

    return true;
}

/*! tcp_client::connect - creates socket and connects.
 *
 * creates socket and initiates tcp connection.
 */
bool tcp_client::connect(const char *host, const char *port) {

    if (!get_addr_info(host, port)) return false;
    if (!this->results) return false;

    bool rc(false);

    for (rp = results; rp != NULL; rp = rp->ai_next) {
        tcp_socket = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (tcp_socket == -1)
            continue;

        if (::connect(tcp_socket, rp->ai_addr, rp->ai_addrlen) != -1) {

            int option(1);
            setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &option, sizeof (option));

            rc = true;

            syslog(LOG_DEBUG, "connection to %s OK", host);

            break;
        }

        close(tcp_socket);
    }

    if (rp == NULL) {
        syslog(LOG_DEBUG, "unable to allocate interface to destination host");
        return false;
    }

    freeaddrinfo(results);

    if (NULL == (this->tx = fdopen(tcp_socket, "w"))) {
        close(tcp_socket);
        syslog(LOG_DEBUG, "unable to open TX stream");
    } else rc = true;

    if (NULL == (this->rx = fdopen(tcp_socket, "r"))) {
        close(tcp_socket);
        syslog(LOG_DEBUG, "unable to open RX stream");
    } else rc = true;

    return rc;
}

void tcp_client::disconnect(void) {
    close(this->tcp_socket);
    if (tx)fclose(tx);
    if (rx)fclose(rx);

    rx = NULL;
    tx = NULL;
    this->tcp_socket = 0;
}
/*! tcp_client::set_tx_buffer_size - increase tx buffer size.
 *
 * size - the amount of data in bytes the buffer will store
 * before flush().
 */
bool tcp_client::set_bgp_tx_buffer_size(const size_t &size) {
    bgp_tx_buffer_size = size;
    return !setvbuf(this->tx, NULL, _IOFBF, size);
}
/*! tcp_client::set_rx_buffer_size - increase tx buffer size.
 *
 * size - the amount of data in bytes the buffer will store
 * before flush().
 */
bool tcp_client::set_bgp_rx_buffer_size(const size_t &size) {
    bgp_rx_buffer_size = size;
    return !setvbuf(this->rx, NULL, _IOFBF, size);
}

bool tcp_client::set_tcp_tx_buffer_size(const size_t &size) {

    tcp_tx_socket_buffer_size = size;
    bool ret_val(false);
    int option(size);

    ret_val = !setsockopt(tcp_socket, SOL_SOCKET, SO_SNDBUF,
            (char *) &option, sizeof (option));

    return ret_val;
}

bool tcp_client::set_tcp_rx_buffer_size(const size_t &size) {

    tcp_rx_socket_buffer_size = size;
    bool ret_val(false);
    int option(size);

    ret_val = !setsockopt(tcp_socket, SOL_SOCKET, SO_RCVBUF,
            (char *) &option, sizeof (option));

    return ret_val;
}
/*!tcp_client::set_rx_buffer_size - resize bgp and tcp socket buffers
 */
bool tcp_client::set_tcp_bgp_rx_buffer_size(const size_t &tcp_buffer_size,
        const size_t &bgp_buffer_size) {

    tcp_rx_socket_buffer_size = tcp_buffer_size;
    bgp_rx_buffer_size = bgp_buffer_size;
    bool ret_val(false);
    int option(tcp_buffer_size);

    ret_val = !setsockopt(tcp_socket, SOL_SOCKET, SO_RCVBUF,
            (char *) &option, sizeof (option));

    ret_val = !setvbuf(this->rx, NULL, _IOFBF, bgp_buffer_size);

    return ret_val;
}
/*!tcp_client::set_tx_buffer_size - resize bgp and tcp socket buffers
 */
bool tcp_client::set_tcp_bgp_tx_buffer_size(const size_t &tcp_buffer_size,
        const size_t &bgp_buffer_size) {

    tcp_tx_socket_buffer_size = tcp_buffer_size;
    bgp_tx_buffer_size = bgp_buffer_size;
    bool ret_val(false);
    int option(tcp_buffer_size);

    ret_val = !setsockopt(tcp_socket, SOL_SOCKET, SO_SNDBUF,
            (char *) &option, sizeof (option));

    ret_val = !setvbuf(this->tx, NULL, _IOFBF, bgp_buffer_size);

    return ret_val;
}

int tcp_client::get_tcp_rx_buffer_size(void) {

    int sockbufsize(0);
    socklen_t size(sizeof (int));

    int ret_val = getsockopt(this->tcp_socket, SOL_SOCKET, SO_RCVBUF,
            (char *) &sockbufsize, &size);

    if (ret_val < 0)
        perror("getsockopt");

    return sockbufsize;
}

int tcp_client::get_tcp_tx_buffer_size(void) {

    int sockbufsize(0);
    socklen_t size(sizeof (int));

    int ret_val = getsockopt(this->tcp_socket, SOL_SOCKET, SO_SNDBUF,
            (char *) &sockbufsize, &size);

    if (ret_val < 0)
        perror("getsockopt");

    return sockbufsize;
}

/*! tcp_client::connected - test eof on streams.
 *
 * if either FILE stream at eof returns true.
 */
bool tcp_client::connected(void) {

    if (rx == NULL) return false;
    if (tx == NULL) return false;

    this->lock_feof();
    bool connected_((!feof(rx) && !feof(tx)));
    this->unlock_feof();

    return connected_;
}

/*! tcp_client::read - read from rx stream.
 *
 * calls fread() on rx stream.
 */
size_t tcp_client::read(void *data, const size_t &size,
        const size_t &count) {

    if (!connected()) {
        syslog(LOG_DEBUG, "can not read, no stream available");
        return -1;
    }

    size_t read_(fread(data, size, count, this->rx));

    return read_;
}

int tcp_client::read_char(void) {

    if (!connected()) {
        syslog(LOG_DEBUG, "can not read, no stream available");
        return -1;
    }

    int read_(fgetc(this->rx));

    return read_;
}

/*! tcp_client::write - write tx stream.
 *
 * calls fwrite() on tx stream.
 */
size_t tcp_client::write(void *data, const size_t &size,
        const size_t &count) {

    if (!connected()) {
        syslog(LOG_DEBUG, "can not write, no stream available");
        return -1;
    }

    this->lock_write();
    size_t write_(fwrite(data, size, count, this->tx));
    this->unlock_write();

    return write_;
}

/*! tcp_client::write_formated - format data to stream.
 *
 * calls fprintf() on tx stream.
 */
int tcp_client::write_formated(const char *format, const int &a,
        const int &b) {

    if (!connected()) {
        syslog(LOG_DEBUG, "can not write_formated, no stream available");
        return -1;
    }

    this->lock_write();
    int write_(fprintf(this->tx, format, a, b));
    this->unlock_write();

    return write_;
}

/*! tcp_client::tx_flush - flush tx stream.
 *
 * calls fflush() on tx stream to send data
 * and empty buffer.
 */
int tcp_client::tx_flush(void) {
    this->lock_write();
    int rc(fflush(this->tx));
    this->unlock_write();

    return rc;
}

/*! tcp_client::rx_flush - flush rx stream.
 *
 * calls fflush() on rx stream to send data
 * and empty buffer.
 */
int tcp_client::rx_flush(void) {
    int rc(fflush(this->rx));
    return rc;
}