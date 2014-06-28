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

#ifndef UDP_CLIENT_H
#define	UDP_CLIENT_H

#include <string>
#include <map>
#include <mutex>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct udp_sort {

    bool operator()(const uint32_t &a, const uint32_t &b) {
        return a < b;
    }
};

class udp_sender {
public:
    uint32_t __ipv4;
private:

    enum class msg_length : uint16_t {
        max = 512
    };

    uint16_t __port;
    std::string __host;
    int __socket;
    int __sequence;

    sockaddr_in __dest_;

public:

    const std::string &host(void) const {
        return __host;
    }

    const uint16_t &port(void) const {
        return __port;
    }

    udp_sender() :
    __ipv4(0), __port(0), __host(""), __socket(0), __sequence(1000) {
    }

    void operator()(
            const uint32_t &ipv4,
            const std::string &host,
            const uint16_t &port) {

        __ipv4 = ipv4;
        __port = port;
        __host = host;

        if ((__socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            syslog(LOG_DEBUG,
                    "udp: unable to allocate UDP socket");
        } else {
            memset(&__dest_, 0, sizeof (__dest_));
            __dest_.sin_family = AF_INET;
            __dest_.sin_port = htons(__port);

            if (inet_aton(__host.c_str(), &__dest_.sin_addr) < 0) {
                syslog(LOG_DEBUG, "udp: invalid ipv4 address");
            }
        }
    }

    ~udp_sender() {
        if (__socket != 0)close(__socket);
    }

    void write(const uint8_t &facility, const uint8_t &priority,
            const std::string &data) {

        std::stringstream __packet;
        __packet.clear();

        __packet << '<' << (facility | priority) << '>'
                << ++__sequence << ':';

        __packet << data;

        int len(__packet.str().length());

        if (sendto(__socket,
                __packet.str().c_str(),
                len,
                0, (const sockaddr *) &__dest_,
                sizeof (__dest_)) < 0) {

            syslog(LOG_DEBUG,
                    "udp: failed to write data to %s",
                    __host.c_str());
        }
    }
};

// [peer ipv4] = udp_sender information
typedef std::map<uint32_t, udp_sender, udp_sort> udp_destinations;

#endif	/* UDP_CLIENT_H */

