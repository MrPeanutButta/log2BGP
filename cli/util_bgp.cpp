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

#include <arpa/inet.h>
#include <iostream>
#include "util.h"
#include "tcp_client.h"
#include "cmdparser.h"
#include "misc.h"

using namespace std;

namespace Utility {
    const std::string Util::bgp_path = "bigplg ";
    tcp_client *Util::bgp;
    bool bgp_connected = false;
}

using namespace Utility;

#define LOCALHOST "127.0.0.1"
#define BGP_PROC_PORT "666"

void Util::init_bgp_proc_pipe(void) {
    bgp = new tcp_client;

    try {

        if (!bgp->connect(LOCALHOST, BGP_PROC_PORT)) {
            delete bgp;
            bgp = nullptr;
        }

    } catch (...) {
    }

}

void Util::read_from_bgp(void) {

    char *buffer{};
    size_t bytes(0);
    int len(0);

    len = (bgp->read_char() << 24 | bgp->read_char() << 16
            | bgp->read_char() << 8 | bgp->read_char());

    if (len == 0)return;

    buffer = new char [len + 1];

    if (buffer != NULL) {
        bytes = bgp->read(buffer, 1, len);
        buffer[bytes] = '\0';
        more(buffer);
    }

    delete [] buffer;

    buffer = NULL;
}

bool Util::write_to_bgp(std::string cmd) {

    bool written = true;

    int len = cmd.length();
    bgp->write(&len, 1, 1);
    bgp->write(cmd.c_str(), 1, len);
    bgp->tx_flush();

    cmd.clear();

    return written;
}

int Util::proto_bgp_set_peer_logging(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::show_bgp_prefix(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    in_addr n_ip;

    if (inet_aton(tokens[3].c_str(), &n_ip)) {

        string cmd;

        for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

        if (write_to_bgp(cmd))
            read_from_bgp();

        tokens.clear();
        return 0;
    } else std::cerr << tokens[3] + " is not a valid IPv4 prefix\n";

    tokens.clear();
    return 0;
}

int Util::show_bgp_prefix_w_len(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_toggle_update_dump(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_kill_all(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    delete bgp;

    tokens.clear();
    return 0;
}

int Util::ProtoBGP(std::vector<std::string> &tokens) {

    if (tokens.size() < 3) {

        cerr << "bgp: requires argument 'AS #'\n";
        tokens.clear();
        return -1;

    } else if (tokens.size() < 3) {

        cerr << "bgp: too many arguments\n";
        tokens.clear();
        return -1;

    }

    for (int i = 0; i < tokens[2].length(); i++) {

        if (!isdigit(tokens[2].c_str()[i])) {

            cerr << "bgp: AS argument must be an integer\n";
            tokens.clear();
            return -1;

        }

    }

    int range = atoi(tokens[2].c_str());

    if (range < 1 || range > 65535) {

        cerr << "bgp: invalid AS range\n";
        tokens.clear();
        return -1;

    }

    if (bgp != nullptr) {

        p_s.EnterConfigProtoBGP();
        tokens.clear();
        return 0;

    } else {

        string bgpd = bgp_path + tokens[2];
        system(bgpd.c_str());
        usleep(5000);
        init_bgp_proc_pipe();

    }

    p_s.EnterConfigProtoBGP();
    tokens.clear();

    return 0;
}

int Util::ProtoBGPNeighbor(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 3) {

        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;

    }

    in_addr n_ip;
    /*
        if (!inet_aton(tokens[1].c_str(), &n_ip)) {
            cerr << "neighbor: IP " << tokens[1] << " is invalid\n";
            tokens.clear();
            return -1;
        }*/

    for (int i = 0; i < tokens[3].length(); i++) {

        if (!isdigit(tokens[3].c_str()[i])) {

            cerr << "bgp: AS argument must be an integer\n";
            tokens.clear();
            return -1;

        }

    }

    int range = atoi(tokens[3].c_str());

    if (range < 1 || range > 65535) {
        cerr << "bgp: invalid AS range\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::BGPID(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 2) {
        cerr << "bgp-id: requires an argument\n";
        tokens.clear();
        return -1;
    }

    in_addr n_ip;

    if (!inet_aton(tokens[1].c_str(), &n_ip)) {
        cerr << "bgp-id: ID " << tokens[1] << " is invalid\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++) cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::ShowBGPNeighbors(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++) cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();

    return 0;
}

int Util::ShowBGPAdjRibIn(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::ShowBGPAdjRibWithdrawn(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_peer_timers(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_peer_attribute(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_bgp_id_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 5) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_origin_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 5) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();


    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_aspath_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 7) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_nexthop_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_v4nexthop_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 7) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();


    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_v6nexthop_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_med_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_localpref_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_atomicaggr_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    tokens.clear();
    return 0;
}

int Util::proto_bgp_attribute_community_translate_value(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 7) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();


    tokens.clear();
    return 0;
}

int Util::proto_bgp_peer_rcv_buffer_size(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    int range1 = atoi(tokens[3].c_str());
    int range2 = atoi(tokens[4].c_str());

    //tcp socket <512-131072>"
    if (range1 < 512 || range1 > 131072) {
        cerr << "invalid receive buffer size\n";
        tokens.clear();
        return -1;
    }
    //bgp buffer <512-131072>"
    if (range2 < 512 || range2 > 131072) {
        cerr << "invalid receive buffer size\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

int Util::proto_bgp_peer_snd_buffer_size(std::vector<std::string> &tokens) {
    if (bgp == nullptr) return 0;
    if (!bgp->connected())return 0;

    if (tokens.size() < 4) {
        cerr << "neighbor: requires arguments\n";
        tokens.clear();
        return -1;
    }

    int range1 = atoi(tokens[3].c_str());
    int range2 = atoi(tokens[4].c_str());

    //tcp socket <512-131072>"
    if (range1 < 512 || range1 > 131072) {
        cerr << "invalid receive buffer size\n";
        tokens.clear();
        return -1;
    }
    //bgp buffer <512-131072>"
    if (range2 < 512 || range2 > 131072) {
        cerr << "invalid receive buffer size\n";
        tokens.clear();
        return -1;
    }

    string cmd;

    for (int i = 0; i < tokens.size(); i++)cmd += tokens[i] + " ";

    if (write_to_bgp(cmd))
        read_from_bgp();

    tokens.clear();
    return 0;
}

