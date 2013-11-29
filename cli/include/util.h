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

#ifndef UTIL_H
#define	UTIL_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "cmdparser.h"
#include "tcp_client.h"

using namespace CommandParser;

//class CmdParser;

/// Utility class for command interfacing

class Util : CmdParser {
public:

    static int HostName(std::vector<std::string> &);
    static int Enable(std::vector<std::string> &);
    static int Exit(std::vector<std::string> &);
    static int End(std::vector<std::string> &);
    static int ProtoBGP(std::vector<std::string> &);
    static int Proto(std::vector<std::string> &);
    static int Configure(std::vector<std::string> &);
    static int ConfigureTerminal(std::vector<std::string> &);
    static int ConfigureSubSystem(std::vector<std::string> &);
    static int Disable(std::vector<std::string> &);
    static int Show(std::vector<std::string> &);
    static int CD(std::vector<std::string> &);
    static int PWD(std::vector<std::string> &);
    static int Delete(std::vector<std::string> &);
    static int Rename(std::vector<std::string> &);
    static int List(std::vector<std::string> &);
    static int Ping(std::vector<std::string> &);
    static int Stat(std::vector<std::string> &);
    static int Help(std::vector<std::string> &);
    static int QMark(std::vector<std::string> &);
    static int ProtoBGPNeighbor(std::vector<std::string> &);
    static int BGPID(std::vector<std::string> &);
    static int ShowBGPNeighbors(std::vector<std::string> &);
    static int WriteConfigs(std::vector<std::string> &);
    static void InitProcPipes(void);
    static int proto_bgp_set_peer_logging(std::vector<std::string> &);
    static int proto_bgp_peer_timers(std::vector<std::string> &);
    static int proto_bgp_peer_attribute(std::vector<std::string> &);
    static int proto_bgp_attribute_bgp_id_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_origin_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_aspath_translate_value(std::vector<std::string> &);

    static int proto_bgp_attribute_nexthop_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_v4nexthop_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_v6nexthop_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_med_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_localpref_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_atomicaggr_translate_value(std::vector<std::string> &);
    static int proto_bgp_attribute_community_translate_value(std::vector<std::string> &);

    static int proto_bgp_peer_rcv_buffer_size(std::vector<std::string> &);
    static int proto_bgp_peer_snd_buffer_size(std::vector<std::string> &);

    static int proto_bgp_kill_all(std::vector<std::string> &);

    static int proto_bgp_toggle_update_dump(std::vector<std::string> &);

    static int ShowBGPAdjRibIn(std::vector<std::string> &);
    static int ShowBGPAdjRibWithdrawn(std::vector<std::string> &);
    static int show_bgp_prefix(std::vector<std::string> &);
    static int show_bgp_prefix_w_len(std::vector<std::string> &);

    static void exiting(void);

    explicit Util();

    static const std::string bgp_path;
    static tcp_client *bgp;

private:

    static void QMarkDisplayAvailable(std::string &, std::string &);
    static void QMarkDisplayAvailable(const std::string &, const std::string &);
    static void QMarkDisplayAllInMode(void);
    static bool write_to_bgp(std::string);

    static void read_from_bgp(void);
    static void init_bgp_proc_pipe(void);

    static const uint8_t max_cmd_len = 0xff;
};

#endif	/* UTIL_H */

