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

#ifndef BGP_SYSLOG_H
#define	BGP_SYSLOG_H

#include <sstream>
#include <fstream>
#include "bgp_msg.h"
#include "bgp_peer.h"
#include "udp_sender.h"

class bgp_syslog {
public:

    bgp_syslog() {

    }

    bgp_syslog(const bgp_syslog& orig);

    virtual ~bgp_syslog() {

    };

    void translate_and_dump_update(BGP *, const bgp_update &, nlri_parse);

    // key == peer ipv4 addr
    void add_dest_host(const uint32_t &key,
            const std::string &dest_host_ipv4,
            const uint16_t &port);

    udp_destinations &get_udp_destinations(void);

private:

    udp_destinations __udp_map;
    std::ofstream dumper;

};

#endif	/* BGP_SYSLOG_H */

