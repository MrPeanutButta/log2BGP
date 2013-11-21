#include <iomanip>
#include <fstream>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <time.h>
#include "bgp_msg.h"
#include "bgp.h"
#include "bgp_adj_rib.h"
#include "misc.h"
#include "bgp_database.h"

/** API - GET
 *
 * matches:
 *      /bgp/ipv4/:prefix
 *      /bgp/ipv4/:prefix/history
 *
 * @param bgp    : main bgp instance
 * @param tokens : commands vertorized by cmd parser
 * @return       : json formatted sting of bgp prefix data
 */
std::string BGP::api_get_prefix(BGP *bgp, std::vector<std::string> &tokens) {

    std::stringstream s_s;
    s_s.clear();

    s_s << "{\"entries\":[";

    in_addr n_ip;
    bool dump_history = false;
    records ret_rec;

    if (!inet_aton(tokens[2].c_str(), &n_ip)) {
        tokens.clear();
        s_s << "{\"prefix\":\"c'mon, thats not even a real ip prefix\"}]}";
        return s_s.str();
    }

    // container to hold matches
    std::vector<bgp_update> prefix_matches;

    if (tokens.size() == 4) {
        if (!tokens[3].compare(0, tokens[3].length(), "history")) {
            dump_history = true;
        }
    }

    /* if we don't lock and the table is active removing/inserting
     * routes, SEGFAULT is expected to occur */
    bgp->lock_adj_rib_in();

    // search for all matches
    for (const auto &entry : bgp->get_adj_rib_in()) {

        // if n_ip.s_addr is in subnet range, it's a match
        if (memcmp(
                &entry.second.nlri.prefix, &n_ip.s_addr, sizeof (uint32_t)
                ) <= 0 &&
                memcmp(
                &entry.second.nlri.prefix_highest, &n_ip.s_addr, sizeof (uint32_t)
                ) >= 0) {

            // save copy of match
            prefix_matches.emplace_back(entry.second);
        }
    }

    bgp->unlock_adj_rib_in();

    if (prefix_matches.size() == 0) {
        tokens.clear();
        s_s << "{\"prefix\":\"network not in table\"}]}";
        return s_s.str();
    }

    int pos = 0;
    for (bgp_update &entry : prefix_matches) {

        ++pos;
        // jsonify matches
        s_s << "{\"prefix\":\"" << ip_to_string(entry.nlri.prefix) << "\",";
        s_s << "\"prefix_bin\":" << (unsigned) entry.nlri.prefix << ',';
        s_s << "\"netmask\":\"" << ip_to_string(htonl((0xffffffff << (32 - entry.nlri.prefix_length)))) << "\",";
        s_s << "\"netmask_bin\":" << (unsigned) htonl((0xffffffff << (32 - entry.nlri.prefix_length))) << ',';
        s_s << "\"length\":" << (unsigned) entry.nlri.prefix_length << ',';
        s_s << "\"prefix_range\":\"" << ip_to_string(entry.nlri.prefix_highest) << "\",";
        s_s << "\"prefix_range_bin\":" << entry.nlri.prefix_highest << ",";
        s_s << "\"peer\":\"" << ip_to_string(entry.nlri.bgp_id) << '\"';

        if (bgp->peer_doing_alias(entry.nlri.bgp_id)) {
            s_s << ",\"peer_name\":\"" << bgp->translate_bgp_id(entry.nlri.bgp_id) << "\",";
        } else {
            s_s << ",";
        }

        s_s << "\"current\":{" << bgp->jsonify_entry(&entry) << "}";

        if (dump_history) {
            ret_rec = bgp->db_get_entries(bgp, entry);
            s_s << ",\"history_length\":" << ret_rec.size();
            s_s << ",\"history\":[";
            bgp_update *update = nullptr;
            for (auto rec = ret_rec.begin(); rec != ret_rec.end(); ++rec) {
                update = &*rec;
                s_s << "{" << bgp->jsonify_entry(update);

                // determine if last entry
                if ((rec + 1) != ret_rec.end()) {
                    s_s << "},";
                } else {
                    s_s << "}";
                }
            }
            // close the array
            s_s << "]";
        }

        if (pos != prefix_matches.size()) {
            s_s << "},";
        } else {
            s_s << "}";
        }
    }

    s_s << "]}";
    return s_s.str();
}

struct prefix_matches {
    uint32_t ip;
    uint8_t length;
};

/**
 * matches:
 *      '/bgp/ipv4/all/from-asn/:asn/:length'
 *
 * @param bgp    : main bgp instance
 * @param tokens : "api-get all-from-asn :asn :length"
 * @return a JSON array of prefixes owned by ASN
 */
std::string BGP::api_get_all_origin_as(BGP* bgp, std::vector<std::string>& tokens) {

    bgp->lock_adj_rib_in();

    uint32_t asn = string_to_uint32_t(tokens[2]);
    uint32_t length = string_to_uint32_t(tokens[3]);

    // container to hold matches
    std::vector<prefix_matches> ip_matches;

    uint8_t len = 0;
    for (bgp_adj_rib::iterator it_entry = bgp->get_adj_rib_in().begin();
            it_entry != bgp->get_adj_rib_in().end(); ++it_entry) {

        if (it_entry->second.as_path != nullptr) {
            len = (it_entry->second.as_path->length - 1);

            // we only want blocks larger than or equal too /24
            if (it_entry->second.nlri.prefix_length <= length) {
                if (it_entry->second.as_path->seg_value[len] == asn) {
                    ip_matches.push_back({it_entry->second.nlri.prefix,
                        it_entry->second.nlri.prefix_length});
                }
            }
        }
    }

    bgp->unlock_adj_rib_in();

    std::stringstream s_s;
    s_s.clear();

    s_s << "{\"entries\":[";

    if (!ip_matches.empty()) {
        std::vector<prefix_matches>::iterator it_match;

        for (it_match = ip_matches.begin();
                it_match != ip_matches.end();
                ++it_match) {

            s_s << "\"" << ip_to_string(it_match->ip) << '/'
                    << (int) it_match->length << "\"";

            if ((it_match + 1) != ip_matches.end()) {
                s_s << ",";
            }
        }
    }

    s_s << "]}";

    return s_s.str();
}

/** JSON format BGP attributes
 *
 * @param bgp   : man BGP instance
 * @param entry : BGP RIB entry (bgp_update)
 * @return      : JSON formatted string of RIB entry
 */
std::string BGP::jsonify_entry(bgp_update *entry) {

    std::stringstream s_s;
    s_s.clear();

    if (entry == nullptr) {
        return s_s.str();
    }

    char t_buff[80];
    strftime(t_buff, 80, "%F %T", gmtime(&entry->nlri.age));
    s_s << "\"time_str\":\"" << t_buff << "\"";

    s_s << ",\"time_epoch\":" << entry->nlri.age;

    if (entry->as_path != nullptr) {
        s_s << ",\"as_path_length\":" << (int) entry->as_path->length;
        s_s << ",\"as_path\":[";
        for (uint8_t i = 0; i < entry->as_path->length; ++i) {
            s_s << "\"" << this->translate_as_path(entry->nlri.bgp_id,
                    entry->as_path->seg_value[i]) << "\"";

            if ((i + 1) < entry->as_path->length) {
                s_s << ",";
            }
        }
        s_s << "]";
    }

    if (entry->as4_path != nullptr) {
        s_s << ",\"as4_path_length\":" << (int) entry->as4_path->length;
        s_s << ",\"as4_path\":[";
        for (uint8_t i = 0; i < entry->as4_path->length; ++i) {
            s_s << "\"" << this->translate_as_path(entry->nlri.bgp_id,
                    entry->as4_path->seg_value[i]);

            if ((i + 1) < entry->as4_path->length) {
                s_s << ",";
            }
        }
        s_s << "]";
    }

    s_s << ",\"origin\":\"" << bgp_origin_code_text[entry->origin] << "\",";

    s_s << "\"next_hop_str\":\"" << ip_to_string(entry->ipv4_next_hop) << "\",";
    s_s << "\"next_hop_name\":\"" << this->translate_ipv4_next_hop(
            entry->nlri.bgp_id, entry->ipv4_next_hop) << "\",";

    s_s << "\"next_hop_bin\":" << entry->ipv4_next_hop << ",";
    s_s << "\"med\":" << entry->med << ",";
    s_s << "\"local_pref\":" << entry->local_pref;

    if (entry->atomic_aggregate) {
        s_s << ",\"atomic_aggregate\":" << "true";
    } else {
        s_s << ",\"atomic_aggregate\":" << "false";
    }

    if (entry->aggregator != nullptr) {
        s_s << ",\"aggregator_str\":\"" << this->translate_aggregator(entry->nlri.bgp_id,
                entry->aggregator->origin) << "\",";
        s_s << "\"aggregator_bin\":" << entry->aggregator->origin << ",";

        s_s << "\"aggregator_as_str\":\"" << this->translate_as_path(entry->nlri.bgp_id,
                entry->aggregator->as) << "\",";
        s_s << "\"aggregator_as_bin\":" << entry->aggregator->as;
    }

    if (entry->as4_aggregator != nullptr) {
        s_s << ",\"as4_aggregator_str\":\"" << this->translate_aggregator(entry->nlri.bgp_id,
                entry->as4_aggregator->origin) << "\",";
        s_s << "\"as4_aggregator_bin\":" << entry->as4_aggregator->origin << ",";

        s_s << "\"as4_aggregator_as_str\":\"" << this->translate_as_path(entry->nlri.bgp_id,
                entry->as4_aggregator->as) << "\",";
        s_s << "\"as4_aggregator_as_bin\":" << entry->aggregator->as;
    }

    if (entry->com_seg_length > 0) {
        if (entry->community != nullptr) {
            s_s << ",\"community_length\":" << (int) entry->com_seg_length;
            s_s << ",\"community\":[";
            for (uint8_t i = 0; i < entry->com_seg_length; ++i) {
                s_s << "\"" << this->translate_as_path(entry->nlri.bgp_id,
                        entry->community[i].as);
                s_s << ":";
                s_s << this->translate_community_value(entry->nlri.bgp_id,
                        entry->community[i].value) << "\"";

                if ((i + 1) < entry->com_seg_length) {
                    s_s << ",";
                }

            }
            s_s << "]";
        }
    }

    return s_s.str();
}

struct ip_class {
    uint32_t prefix;
    uint32_t range;
    uint8_t length;
};

struct rfc1918 {
    std::string ip;
    uint8_t length;
};

const rfc1918 rfc_ips[] = {
    {"10.0.0.0", 8},
    {"172.16.0.0", 12},
    {"192.168.0.0", 16}
};

extern uint32_t get_ipv4_high_range(const uint32_t &, const uint8_t &);

/**
 * checks if the passed prefix is RFC1918
 *
 * @param prefix : ipv4 prefix to check
 * @return true if 1918
 */
bool is_rfc1918(const uint32_t &prefix) {

    in_addr n_ip; // reused
    ip_class ipc[3];

    // prepare our structures
    for (int i = 0; i < 3; ++i) {
        inet_aton(rfc_ips[i].ip.c_str(), &n_ip);
        ipc[i].prefix = n_ip.s_addr;
        ipc[i].length = rfc_ips[i].length;
        ipc[i].range = get_ipv4_high_range(ipc[i].prefix, ipc[i].length);
    }

    // perform the range check
    for (const auto &ip_block : ipc) {
        // if n_ip.s_addr is in subnet range, it's a match
        if (memcmp(&ip_block.prefix, &prefix, sizeof (uint32_t)) <= 0 &&
                memcmp(&ip_block.range, &prefix, sizeof (uint32_t)) >= 0) {
            return true;
        }
    }

    return false;
}

std::string BGP::api_get_public_origin_as(BGP* bgp,
        std::vector<std::string> &tokens) {


    bgp->lock_adj_rib_in();

    uint32_t asn = string_to_uint32_t(tokens[2]);
    uint32_t length = string_to_uint32_t(tokens[3]);

    // container to hold matches
    std::vector<prefix_matches> ip_matches;

    uint8_t len = 0;
    for (bgp_adj_rib::iterator it_entry = bgp->get_adj_rib_in().begin();
            it_entry != bgp->get_adj_rib_in().end(); ++it_entry) {

        if (it_entry->second.as_path != nullptr) {
            len = (it_entry->second.as_path->length - 1);

            // if isn't rfc1918 then lets check the length
            if (!is_rfc1918(it_entry->second.nlri.prefix)) {
                // we only want blocks larger than or equal to length
                if (it_entry->second.nlri.prefix_length <= length) {
                    if (it_entry->second.as_path->seg_value[len] == asn) {
                        ip_matches.push_back({it_entry->second.nlri.prefix,
                            it_entry->second.nlri.prefix_length});
                    }
                }
            }
        }
    }

    bgp->unlock_adj_rib_in();

    std::stringstream s_s;
    s_s.clear();

    s_s << "{\"entries\":[";

    if (!ip_matches.empty()) {
        std::vector<prefix_matches>::iterator it_match;

        for (it_match = ip_matches.begin();
                it_match != ip_matches.end();
                ++it_match) {

            s_s << "\"" << ip_to_string(it_match->ip) << '/'
                    << (int) it_match->length << "\"";

            if ((it_match + 1) != ip_matches.end()) {
                s_s << ",";
            }
        }
    }

    s_s << "]}";

    return s_s.str();

}

std::string BGP::api_get_private_origin_as(BGP* bgp,
        std::vector<std::string> &tokens) {


    bgp->lock_adj_rib_in();

    uint32_t asn = string_to_uint32_t(tokens[2]);
    uint32_t length = string_to_uint32_t(tokens[3]);

    // container to hold matches
    std::vector<prefix_matches> ip_matches;

    uint8_t len = 0;
    for (bgp_adj_rib::iterator it_entry = bgp->get_adj_rib_in().begin();
            it_entry != bgp->get_adj_rib_in().end(); ++it_entry) {

        if (it_entry->second.as_path != nullptr) {
            len = (it_entry->second.as_path->length - 1);

            // if is rfc1918 then lets check the length
            if (is_rfc1918(it_entry->second.nlri.prefix)) {
                // we only want blocks larger than or equal to length
                if (it_entry->second.nlri.prefix_length <= length) {
                    if (it_entry->second.as_path->seg_value[len] == asn) {
                        ip_matches.push_back({it_entry->second.nlri.prefix,
                            it_entry->second.nlri.prefix_length});
                    }
                }
            }
        }
    }

    bgp->unlock_adj_rib_in();

    std::stringstream s_s;
    s_s.clear();

    s_s << "{\"entries\":[";

    if (!ip_matches.empty()) {
        std::vector<prefix_matches>::iterator it_match;

        for (it_match = ip_matches.begin();
                it_match != ip_matches.end();
                ++it_match) {

            s_s << "\"" << ip_to_string(it_match->ip) << '/'
                    << (int) it_match->length << "\"";

            if ((it_match + 1) != ip_matches.end()) {
                s_s << ",";
            }
        }
    }

    s_s << "]}";

    return s_s.str();

}

