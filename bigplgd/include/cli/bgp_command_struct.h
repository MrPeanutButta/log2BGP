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


COMMAND cmds[] = {
    { "api-get", BGP::api_get_prefix,
        {
            {"prefix", BGP::api_get_prefix,
                {
                    {"<x.x.x.x>", BGP::api_get_prefix}
                }},
            {"all-from-asn", BGP::api_get_all_origin_as,
                {
                    {"<ASN>", BGP::api_get_all_origin_as,
                        {
                            {"<LENGTH>", BGP::api_get_all_origin_as}
                        }}
                }},
            {"all-public-asn", BGP::api_get_public_origin_as,
                {
                    {"<ASN>", BGP::api_get_public_origin_as,
                        {
                            {"<LENGTH>", BGP::api_get_public_origin_as}
                        }}
                }},
            {"all-private-asn", BGP::api_get_private_origin_as,
                {
                    {"<ASN>", BGP::api_get_private_origin_as,
                        {
                            {"<LENGTH>", BGP::api_get_private_origin_as}
                        }}
                }}
        }},
    {
        "show", BGP::Show,
        {
            { "bgp", BGP::Show,
                {
                    {"ipv4", BGP::show_bgp_prefix,
                        {
                            {"<x.x.x.x>", BGP::show_bgp_prefix}
                        }},
                    { "neighbors", BGP::show_bgp_neighbors},
                    { "adj-rib-in", BGP::show_bgp_adj_rib_in},
                    { "rib-withdrawn", BGP::show_bgp_adj_rib_wdrawn}
                }}
        }
    },
    {
        "write", BGP::write_configs
    },
    {
        "dumper", BGP::proto_bgp_toggle_update_dump,
        {
            { "on", BGP::proto_bgp_toggle_update_dump},
            { "off", BGP::proto_bgp_toggle_update_dump}
        }
    },

    /// Available through CONFIGPROTOBGP
    {
        "neighbor", BGP::proto_bgp_neighbor,
        {
            { "<x.x.x.x>", BGP::proto_bgp_neighbor,
                {
                    {"logging", BGP::set_peer_logging,
                        {
                            {"host", BGP::set_peer_logging,
                                {
                                    {"<x.x.x.x>", BGP::set_peer_logging,
                                        {
                                            { "<port>", BGP::set_peer_logging}
                                        }}
                                }}
                        }},
                    {"rx-buffer", BGP::proto_bgp_peer_rcv_buffer_size,
                        {
                            {"<512-131072>", BGP::proto_bgp_peer_rcv_buffer_size,
                                {
                                    {"<512-131072>", BGP::proto_bgp_peer_rcv_buffer_size}
                                }}
                        }},
                    {"tx-buffer", BGP::proto_bgp_peer_snd_buffer_size,
                        {
                            {"<512-131072>", BGP::proto_bgp_peer_snd_buffer_size,
                                {
                                    {"<512-131072>", BGP::proto_bgp_peer_snd_buffer_size}
                                }}
                        }},

                    { "remote-as", BGP::proto_bgp_neighbor,
                        {
                            { "<1-65535>", BGP::proto_bgp_neighbor}
                        }},

                    { "timers", BGP::proto_bgp_peer_timers,
                        {
                            { "<0-65535>", BGP::proto_bgp_peer_timers,
                                {
                                    { "<0-65535>", BGP::proto_bgp_peer_timers}
                                }}
                        }},
                    {"attribute", BGP::proto_bgp_peer_attribute,
                        {
                            {"bgp-id", BGP::proto_bgp_attribute_bgp_id_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_bgp_id_translate_value,
                                        {
                                            {"<value>", BGP::proto_bgp_attribute_bgp_id_translate_value}
                                        }}
                                }},
                            {"origin", BGP::proto_bgp_attribute_origin_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_origin_translate_value}
                                }},
                            {"as-path", BGP::proto_bgp_attribute_aspath_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_aspath_translate_value,
                                        {
                                            {"<value>", BGP::proto_bgp_attribute_aspath_translate_value,
                                                {
                                                    {"<new value>", BGP::proto_bgp_attribute_aspath_translate_value}
                                                }}
                                        }}
                                }},
                            { "next-hop", BGP::proto_bgp_attribute_nexthop_translate_value,
                                {
                                    { "alias", BGP::proto_bgp_attribute_nexthop_translate_value,
                                        {
                                            { "ipv4", BGP::proto_bgp_attribute_v4nexthop_translate_value,
                                                {
                                                    { "<x.x.x.x>", BGP::proto_bgp_attribute_v4nexthop_translate_value,
                                                        {
                                                            { "<value>", BGP::proto_bgp_attribute_v4nexthop_translate_value}
                                                        }}
                                                }},

                                            { "ipv6", BGP::proto_bgp_attribute_v4nexthop_translate_value,
                                                {
                                                    { "<x:x:x:x:x:x:x:>", BGP::proto_bgp_attribute_v4nexthop_translate_value,
                                                        {
                                                            { "<value>", BGP::proto_bgp_attribute_v4nexthop_translate_value}
                                                        }}
                                                }}

                                        }}
                                }}, //next-hop
                            {"med", BGP::proto_bgp_attribute_med_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_med_translate_value,
                                        {
                                            {"<value>", BGP::proto_bgp_attribute_med_translate_value}
                                        }}
                                }}, //med
                            {"local-pref", BGP::proto_bgp_attribute_localpref_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_localpref_translate_value,
                                        {
                                            {"<value>", BGP::proto_bgp_attribute_localpref_translate_value}
                                        }}
                                }}, // local-pref
                            {"atomic-aggr", BGP::proto_bgp_attribute_atomicaggr_translate_value,
                                {
                                    {"alias", BGP::proto_bgp_attribute_atomicaggr_translate_value,
                                        {
                                            {"boolean", BGP::proto_bgp_attribute_atomicaggr_translate_value}
                                        }}
                                }}, // atomic-aggr
                            { "community", BGP::proto_bgp_attribute_community_translate_value,
                                {
                                    { "alias", BGP::proto_bgp_attribute_community_translate_value,
                                        {
                                            { "<0-65535>", BGP::proto_bgp_attribute_community_translate_value,
                                                {
                                                    { "<value>", BGP::proto_bgp_attribute_community_translate_value}
                                                }}
                                        }}
                                }}
                        }}
                }}
        }
    },

    {
        "bgp-id", BGP::set_bgp_id,
        {
            { "<x.x.x.x>", BGP::set_bgp_id,}
        }
    },
    {
        "kill-all", BGP::kill_all_signal,
    },
    {
        "", (proto_bgp::cmd_BGP_func_v)NULL
    }
};