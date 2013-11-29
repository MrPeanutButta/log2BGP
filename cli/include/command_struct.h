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

#include "util.h"

/*!Bit flags for the modes of the commands are available to be set.
 *
 * Multiple modes can be flagged for the command to be available in
 * different exec levels.
 *
 * Available flags are:
 * USEREXEC | PRIVEXEC | CONFIG | CONFIGINT | CONFIGLINE | CONFIGROUTERBGP | UNUSED
 *
 * This will provide flexibility with creating new commands. Only the appropriate flags
 * need to be set.
 *
 * These flags represent 1 byte, and each set 1 bit as follows\n
 * 0x01 ==  USEREXEC        == "00000001"\n
 * 0x02 ==  PRIVEXEC        == "00000010"\n
 * 0x04 ==  CONFIG          == "00000100"\n
 * 0x08 ==  CONFIGINT       == "00001000"\n
 * 0x10 ==  CONFIGLINE      == "00010000"\n
 * 0x20 ==  CONFIGROUTER    == "00100000"\n
 * 0x40 ==  CONFIGROUTERBGP == "01000000"\n
 * SUBCOMMAND = USEREXEC | PRIVEXEC | CONFIG | CONFIGINT | CONFIGLINE | CONFIGPROTO | CONFIGPROTOBGP\n
 */

using namespace Utility;

COMMAND cmds[] = {

#ifndef LOOKING_GLASS_ONLY_CLI

    ///Available through USEREXEC.
    { "enable", Util::Enable, "enter privileged exec mode", USEREXEC,
        {
            { "<cr>", Util::Enable, "", SUBCOMMAND}
        }},
#endif

    { "?", Util::QMark, "synonym for quick/command `help'", USEREXEC | PRIVEXEC | CONFIG | CONFIGINT | CONFIGLINE | CONFIGPROTOBGP},
    { "exit", Util::Exit, "exit current configuration level", USEREXEC | PRIVEXEC | CONFIG | CONFIGINT | CONFIGLINE | CONFIGPROTOBGP,
        {
            { "<cr>", Util::Exit, "", SUBCOMMAND}
        }},

#ifndef LOOKING_GLASS_ONLY_CLI

    {"end", Util::End, "exit configuration", CONFIG | CONFIGINT | CONFIGLINE | CONFIGPROTOBGP,
        {
            {"<cr>", Util::End, "", SUBCOMMAND}
        }},

    ///Available through PRIVEXEC.
    { "disable", Util::Disable, "exit privileged exec mode", PRIVEXEC,
        {
            { "<cr>", Util::Disable, "", SUBCOMMAND}
        }},
    { "configure", Util::Configure, "enter configuration mode", PRIVEXEC,
        {
            { "terminal", Util::ConfigureTerminal, "configure from the terminal", SUBCOMMAND,
                {
                    { "<cr>", Util::ConfigureTerminal, "", SUBCOMMAND}
                }},
            { "sub-system", Util::ConfigureSubSystem, "enter bash prompt for system access", SUBCOMMAND,
                {
                    { "<cr>", Util::ConfigureSubSystem, "", SUBCOMMAND}
                }}
        }},

#endif

    { "show", Util::Show, "show running system information", USEREXEC | PRIVEXEC,
        {
            { "bgp", Util::Show, "show bgp stats", SUBCOMMAND,
                {
                    { "neighbors", Util::ShowBGPNeighbors, "show bgp neighbor stats", SUBCOMMAND,
                        {
                            { "<cr>", Util::ShowBGPNeighbors, "", SUBCOMMAND}
                        }},

#ifndef LOOKING_GLASS_ONLY_CLI


                    { "adj-rib-in", Util::ShowBGPAdjRibIn, "show in-bound pre-filtered adjacency RIB", SUBCOMMAND,
                        {
                            {"<cr>", Util::ShowBGPAdjRibIn, "", SUBCOMMAND}
                        }},

#endif

                    {"ipv4", Util::show_bgp_prefix, "IPv4 network", SUBCOMMAND,
                        {
                            {"<x.x.x.x>", Util::show_bgp_prefix, "IPv4 network", SUBCOMMAND,
                                {
                                    {"history", Util::show_bgp_prefix, "IPv4 BGP history", SUBCOMMAND,
                                    {
                                        {"<cr>", Util::show_bgp_prefix, "", SUBCOMMAND}
                                        }},

                                    {"<cr>", Util::show_bgp_prefix, "", SUBCOMMAND}
                                }}
                        }}
                }}
        }},

#ifndef LOOKING_GLASS_ONLY_CLI

    { "write", Util::WriteConfigs, "write running-config", PRIVEXEC,
        {
            { "<cr>", Util::WriteConfigs, "", SUBCOMMAND}
        }},

    ///Available through CONFIG
    { "protocol", Util::Proto, "enter protocol configuration", CONFIG,
        {
            { "bgp", Util::ProtoBGP, "configure Border Gateway Protocol", SUBCOMMAND,
                {
                    { "<1-65535>", Util::ProtoBGP, "autonomous system number", SUBCOMMAND,
                        {
                            { "<cr>", Util::ProtoBGP, "", SUBCOMMAND}
                        }}
                }}
        }},
    { "hostname", Util::HostName, "change hostname", CONFIG,
        {
            { "<name>", Util::HostName, "hostname", SUBCOMMAND,
                {
                    { "<cr>", Util::HostName, "", SUBCOMMAND}
                }}
        }},


    /// Available through CONFIGPROTOBGP
    { "dumper", Util::proto_bgp_toggle_update_dump, "toggle update dumper", CONFIGPROTOBGP,
        {
            { "on", Util::proto_bgp_toggle_update_dump, "toggle on", SUBCOMMAND,
                {
                    { "<cr>", Util::proto_bgp_toggle_update_dump, "", SUBCOMMAND}
                }},
            { "off", Util::proto_bgp_toggle_update_dump, "toggle off", SUBCOMMAND,
                {
                    { "<cr>", Util::proto_bgp_toggle_update_dump, "", SUBCOMMAND}
                }}
        }},
    { "neighbor", Util::ProtoBGPNeighbor, "configure BGP neighbor", CONFIGPROTOBGP,
        {
            { "<x.x.x.x>", Util::ProtoBGPNeighbor, "peer IP address", SUBCOMMAND,
                {
                    {"logging", Util::proto_bgp_set_peer_logging, "configure syslog for this peer", SUBCOMMAND,
                        {
                            {"host", Util::proto_bgp_set_peer_logging, "configure syslog detination host", SUBCOMMAND,
                                {
                                    {"<x.x.x.x>", Util::proto_bgp_set_peer_logging, "destination host IPv4 address", SUBCOMMAND,
                                        {
                                            { "<port>", Util::proto_bgp_set_peer_logging, "destination syslog port", SUBCOMMAND}
                                        }}
                                }}
                        }},
                    {"rx-buffer", Util::proto_bgp_peer_rcv_buffer_size, "sets the size of the receive buffers for a BGP neighbor", SUBCOMMAND,
                        {
                            {"<512-131072>", Util::proto_bgp_peer_rcv_buffer_size, "size (in bytes) of the receive-side TCP socket buffer", SUBCOMMAND,
                                {
                                    {"<512-131072>", Util::proto_bgp_peer_rcv_buffer_size, "size (in bytes) of the receive-side BGP buffer", SUBCOMMAND,
                                        {
                                            { "<cr>", Util::proto_bgp_peer_rcv_buffer_size, "", SUBCOMMAND}
                                        }}
                                }}
                        }},

                    {"tx-buffer", Util::proto_bgp_peer_snd_buffer_size, "sets the size of the send buffers for a BGP neighbor", SUBCOMMAND,
                        {
                            {"<512-131072>", Util::proto_bgp_peer_snd_buffer_size, "size (in bytes) of the send-side TCP socket buffer", SUBCOMMAND,
                                {
                                    {"<512-131072>", Util::proto_bgp_peer_snd_buffer_size, "size (in bytes) of the send-side BGP buffer", SUBCOMMAND,
                                        {
                                            { "<cr>", Util::proto_bgp_peer_snd_buffer_size, "", SUBCOMMAND}
                                        }}
                                }}
                        }},

                    { "remote-as", Util::ProtoBGPNeighbor, "remote autonomous system ", SUBCOMMAND,
                        {
                            { "<1-65535>", Util::ProtoBGPNeighbor, "autonomous system number", SUBCOMMAND,
                                {
                                    { "<cr>", Util::ProtoBGPNeighbor, "", SUBCOMMAND}
                                }}
                        }},

                    { "timers", Util::proto_bgp_peer_timers, "set hold and keep alive timer (NOT IMPLEMENTED)", SUBCOMMAND,
                        {
                            { "<0-65535>", Util::proto_bgp_peer_timers, "keepalive interval", SUBCOMMAND,
                                {
                                    { "<0-65535>", Util::proto_bgp_peer_timers, "holdtime", SUBCOMMAND,
                                        {
                                            { "<cr>", Util::proto_bgp_peer_timers, "", SUBCOMMAND}
                                        }}
                                }}
                        }},
                    {"attribute", Util::proto_bgp_peer_attribute, "match attribute value", SUBCOMMAND,
                        {
                            {"bgp-id", Util::proto_bgp_attribute_bgp_id_translate_value, "match BGP_ID value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_bgp_id_translate_value, "alias BGP_ID value to string", SUBCOMMAND,
                                        {
                                            {"<value>", Util::proto_bgp_attribute_bgp_id_translate_value, "alias BGP_ID value to string value", SUBCOMMAND,
                                                {
                                                    {"<cr>", Util::proto_bgp_attribute_bgp_id_translate_value, "", SUBCOMMAND}
                                                }}
                                        }}
                                }},
                            {"origin", Util::proto_bgp_attribute_origin_translate_value, "match ORIGIN value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_origin_translate_value, "alias ORIGIN value to readable", SUBCOMMAND,
                                        {
                                            {"<cr>", Util::proto_bgp_attribute_origin_translate_value, "", SUBCOMMAND}
                                        }}
                                }},
                            {"as-path", Util::proto_bgp_attribute_aspath_translate_value, "match AS_PATH value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_aspath_translate_value, "alias AS_PATH value", SUBCOMMAND,
                                        {
                                            {"<value>", Util::proto_bgp_attribute_aspath_translate_value, "alias AS within the AS_PATH to some value", SUBCOMMAND,
                                                {
                                                    {"<new value>", Util::proto_bgp_attribute_aspath_translate_value, "alias AS within the AS_PATH to this value", SUBCOMMAND,
                                                        {
                                                            { "<cr>", Util::proto_bgp_attribute_aspath_translate_value, "", SUBCOMMAND}
                                                        }}
                                                }}
                                        }}
                                }},
                            { "next-hop", Util::proto_bgp_attribute_nexthop_translate_value, "match NEXT_HOP value", SUBCOMMAND,
                                {
                                    { "alias", Util::proto_bgp_attribute_nexthop_translate_value, "alias NEXT_HOP value", SUBCOMMAND,
                                        {
                                            { "ipv4", Util::proto_bgp_attribute_v4nexthop_translate_value, "alias IPv4 NEXT_HOP", SUBCOMMAND,
                                                {
                                                    { "<x.x.x.x>", Util::proto_bgp_attribute_v4nexthop_translate_value, "IPv4 NEXT_HOP to translate", SUBCOMMAND,
                                                        {
                                                            { "<value>", Util::proto_bgp_attribute_v4nexthop_translate_value, "alias IPv4 NEXT_HOP to some value", SUBCOMMAND,
                                                                {
                                                                    { "<cr>", Util::proto_bgp_attribute_v4nexthop_translate_value, "", SUBCOMMAND}
                                                                }}
                                                        }}
                                                }},

                                            { "ipv6", Util::proto_bgp_attribute_v4nexthop_translate_value, "* alias IPv6 NEXT_HOP", SUBCOMMAND,
                                                {
                                                    { "<x:x:x:x:x:x:x:>", Util::proto_bgp_attribute_v4nexthop_translate_value, "* IPv6 NEXT_HOP to translate", SUBCOMMAND,
                                                        {
                                                            { "<value>", Util::proto_bgp_attribute_v4nexthop_translate_value, "* alias IPv6 NEXT_HOP to some value", SUBCOMMAND,
                                                                {
                                                                    { "<cr>", Util::proto_bgp_attribute_v4nexthop_translate_value, "", SUBCOMMAND}
                                                                }}
                                                        }}
                                                }}

                                        }}
                                }}, //next-hop
                            {"med", Util::proto_bgp_attribute_med_translate_value, "match MULTI_EXIT_DISC value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_med_translate_value, "alias MULTI_EXIT_DISC value", SUBCOMMAND,
                                        {
                                            {"<value>", Util::proto_bgp_attribute_med_translate_value, "alias MULTI_EXIT_DISC to some value", SUBCOMMAND,
                                                {
                                                    { "<cr>", Util::proto_bgp_attribute_med_translate_value, "", SUBCOMMAND}
                                                }}
                                        }}
                                }}, //med
                            {"local-pref", Util::proto_bgp_attribute_localpref_translate_value, "match LOCAL_PREF value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_localpref_translate_value, "alias LOCAL_PREF value", SUBCOMMAND,
                                        {
                                            {"<value>", Util::proto_bgp_attribute_localpref_translate_value, "alias LOCAL_PREF to some value", SUBCOMMAND,
                                                {
                                                    { "<cr>", Util::proto_bgp_attribute_localpref_translate_value, "", SUBCOMMAND}
                                                }}
                                        }}
                                }}, // local-pref
                            {"atomic-aggr", Util::proto_bgp_attribute_atomicaggr_translate_value, "match ATOMIC_AGGREGATE value", SUBCOMMAND,
                                {
                                    {"alias", Util::proto_bgp_attribute_atomicaggr_translate_value, "alias ATOMIC_AGGREGATE value", SUBCOMMAND,
                                        {
                                            {"boolean", Util::proto_bgp_attribute_atomicaggr_translate_value, "alias ATOMIC_AGGREGATE to boolean true/false", SUBCOMMAND,
                                                {
                                                    { "<cr>", Util::proto_bgp_attribute_atomicaggr_translate_value, "", SUBCOMMAND}
                                                }}
                                        }}
                                }}, // atomic-aggr
                            { "community", Util::proto_bgp_attribute_community_translate_value, "match COMMUNITY value", SUBCOMMAND,
                                {
                                    { "alias", Util::proto_bgp_attribute_community_translate_value, "alias COMMUNITY value", SUBCOMMAND,
                                        {
                                            { "<0-65535>", Util::proto_bgp_attribute_community_translate_value, "match this COMMUNITY value", SUBCOMMAND,
                                                {
                                                    { "<value>", Util::proto_bgp_attribute_community_translate_value, "alias COMMUNITY value to some value", SUBCOMMAND,
                                                        {
                                                            { "<cr>", Util::proto_bgp_attribute_community_translate_value, "", SUBCOMMAND}
                                                        }}
                                                }}
                                        }}
                                }}
                        }}
                }}
        }},

    {
        "bgp-id", Util::BGPID, "set BGP identifier", CONFIGPROTOBGP,
        {
            { "<x.x.x.x>", Util::BGPID, "ID in dotted decimal", SUBCOMMAND,
                {
                    { "<cr>", Util::BGPID, "", SUBCOMMAND}
                }}
        }
    },
    {
        "kill-all", Util::proto_bgp_kill_all, "kill all BGP processes (TESTING ONLY)", CONFIGPROTOBGP,
        {
            { "<cr>", Util::proto_bgp_kill_all, "", SUBCOMMAND}
        }
    },

#endif

    {
        "", (Utility::cmd_util_func_v)NULL, "", USEREXEC
    }
};
