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

#ifndef MSG_H
#define	MSG_H

#include <stdint.h>
#include <vector>
#include <ctime>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include "misc.h"

// BGP message type codes.

enum class bgp_msg_type : uint8_t {
    open = 0x01,
            update = 0x02,
            notification = 0x03,
            keepalive = 0x04,
            routerefresh = 0x05
};

// Capabilities code.

enum class bgp_param_type : uint8_t {
    capabilities = 0x02
};

// Multi-protocol codes.

enum class bgp_multi_proto_capability : uint8_t {
    // multi protocol capability.
    multi_proto_exten = 0x01,
            // multi protocol capability length.
            multi_proto_cap_len = 0x06
};

// IPv4 AFI/SAFI.

enum class bgp_ipv4_addr_family : uint8_t {
    ipv4_addr_fam = 0x01,
            ipv4_unicast = 0x01
};

// IPv6 AFI/SAFI.

enum class bgp_ipv6_addr_family : uint8_t {
    ipv6_addr_fam = 0x02,
            ipv6_unicast = 0x01
};

// Path attribute codes.

enum class bgp_path_attrib : uint8_t {
    // internal use non RFC
    bgp_id = 0x00,
            origin = 0x01,
            as_path = 0x02,
            next_hop = 0x03,
            multi_exit_disc = 0x04,
            local_pref = 0x05,
            atomic_aggregate = 0x06,
            aggregator = 0x07,
            community = 0x08,
            originator = 0x09,
            cluster_list = 0x0A,
            mp_reach_nlri = 0x0E,
            mp_unreach_nlri = 0x0F,
            as4_path = 0x11,
            as4_aggregator = 0x12
};

// origin codes.

enum class bgp_origin_type : uint8_t {
    igp = 0x00,
            egp = 0x01,
            incomplete = 0x02
};

// as_path type codes.

enum class bgp_as_path_type : uint8_t {
    as_set = 0x01,
            as_sequence = 0x02
};

// notification codes.

enum class bgp_error_code : uint8_t {
    msg_head_err = 0x01,
            open_msg_err = 0x02,
            update_msg_err = 0x03,
            hold_time_expired = 0x04,
            finite_state_mach_err = 0x05,
            cease = 0x06
};

// notification sub-codes.

enum class bgp_error_subcode : uint8_t {
    zero = 0x00,
            connection_not_sync = 0x01,
            bad_msg_len = 0x02,
            bad_msg_type = 0x03
};

enum class bgp_open_error_subcode : uint8_t {
    unsup_ver_num = 0x01,
            bad_peer_as = 0x02,
            bad_bgp_id = 0x03,
            unsup_opt_param = 0x04,
            unacpt_hold_time = 0x06,
            unsup_cap = 0x07
};

enum class bgp_rib_codes : uint8_t {
    suppressed = 0x00,
            damped,
            history,
            valid,
            best,
            internal,
            rib_failure,
            stale
};

/*!BGP header
 */
struct bgp_header {
    uint8_t marker[16];
    uint16_t len;
    bgp_msg_type type;
};

/*!BGP auth data
 */
struct bgp_auth {
    uint8_t auth_code;
    uint8_t auth_data[252];
    uint16_t auth_data_len;
};

/*!BGP OPEN message
 */
struct bgp_open {
    uint8_t version; // BGP v4 most likely
    uint16_t AS; // Remote-AS
    uint16_t hold_time; // Time to wait before dropping neighbor
    uint32_t bgp_id; // Router ID
    uint32_t opt_param_len; // Optional parameters length in bytes
};

/*!BGP NOTIFY message
 */
struct bgp_notification {
    bgp_error_code error_code;
    bgp_error_subcode error_subcode;
    uint8_t data[6];
};

/*flags [well-known][transitive][complete][regular length].
 *type code [bgp_path_attrib].
 *length.
 */
struct bgp_attribute {
    uint8_t flags;
    bgp_path_attrib type_code;
    uint8_t length;
};

class as_path_segment {
public:

    as_path_segment() :
    length(0) {
        this->seg_value = nullptr;
    }

    ~as_path_segment() {
        if (this->seg_value != nullptr) {

            delete [] seg_value;
            this->seg_value = nullptr;

        }
    }

    bgp_as_path_type seg_type; // [AS_SET][AS_SEQ]
    uint8_t length; // length of AS_SET | AS_SEQ
    uint32_t *seg_value; // AS_PATH segment value
};

struct aggregator_segment {
public:

    aggregator_segment() :
    as(0), origin(0) {
    }

    uint32_t as;
    uint32_t origin;
};

struct community_segment {
public:

    community_segment() :
    as(0), value(0) {
    }

    uint32_t as;
    uint32_t value;
};

/** Network layer reachability information.
 *
 * bgp_id - BGP ID of peer who sent this.
 * age - time() when route was learned.
 * withdrawn - num times this prefix has been withdrawn in a 24 hour period.
 * received - num times this prefix has been received in a 24 hour period.
 */
struct bgp_nlri {
public:

    bgp_nlri() :
    bgp_id(0), prefix(0),
    prefix_highest(0), subnet_mask(0),
    prefix_length(0), withdrawn(0),
    received(0), age(0),
    last_withdraw_time(0) {
    }

    uint32_t bgp_id;
    // network
    uint32_t prefix;
    // high end of the subnet range
    uint32_t prefix_highest;
    // subnet mask bits
    uint32_t subnet_mask;
    // length
    uint8_t prefix_length;

    uint16_t withdrawn;
    uint16_t received;
    time_t age;
    time_t last_withdraw_time;
};

// Flags for parsing NLRI

enum class nlri_parse : uint8_t {
    update,
    withdraw
};

// Misc. data for update parsing

struct update_data {
    int offset;
    uint16_t read_len;
    bgp_attribute attrib;
};

/** BGP update message.
 *
 * flags - trans. well known. complete.
 * origin - redis, or network statement.
 * as_path - as_path data.
 * as4_path - as4_path data.
 * ipv4_next_hop - IPv4 next_hop.
 * med - metric.
 * local_pref - local AS preference.
 */
class bgp_update {
public:

    bgp_update() :
    flags(0),
    origin(0),
    ipv4_next_hop(0),
    med(0),
    local_pref(0),
    atomic_aggregate(0),
    com_seg_length(0),
    addr_family(0),
    addr_family_id(0),
    mp_nlri_nh_len(0),
    subnetwork_points_of_attachment(0) {

        aggregator = nullptr;
        as4_aggregator = nullptr;
        community = nullptr;
        as_path = nullptr;
        as4_path = nullptr;
    }

    bgp_update(const bgp_update &orig) :
    flags(orig.flags),
    origin(orig.origin),
    ipv4_next_hop(orig.ipv4_next_hop),
    med(orig.med),
    local_pref(orig.local_pref),
    atomic_aggregate(orig.atomic_aggregate),
    com_seg_length(orig.com_seg_length),
    addr_family(orig.addr_family),
    addr_family_id(orig.addr_family_id),
    mp_nlri_nh_len(orig.mp_nlri_nh_len),
    subnetwork_points_of_attachment(orig.subnetwork_points_of_attachment) {

        this->assign(orig);

    }

    ~bgp_update() {
        free_all();
    }

    void assign(const bgp_update &orig) {

        flags = orig.flags;
        origin = orig.origin;
        ipv4_next_hop = orig.ipv4_next_hop;
        med = orig.med;
        local_pref = orig.local_pref;
        atomic_aggregate = orig.atomic_aggregate;
        com_seg_length = orig.com_seg_length;
        addr_family = orig.addr_family;
        addr_family_id = orig.addr_family_id;
        mp_nlri_nh_len = orig.mp_nlri_nh_len;
        subnetwork_points_of_attachment = orig.subnetwork_points_of_attachment;

        aggregator = nullptr;
        as4_aggregator = nullptr;
        community = nullptr;
        as_path = nullptr;
        as4_path = nullptr;

        if (orig.as4_path != nullptr) {
            this->as4_path = new as_path_segment;
            if (this->as4_path != nullptr) {
                this->as4_path->seg_type = orig.as4_path->seg_type;
                this->as4_path->length = orig.as4_path->length;
                if (this->as4_path->length > 0) {

                    this->as4_path->seg_value = new (std::nothrow) uint32_t
                            [this->as4_path->length];

                    if (this->as4_path->seg_value != nullptr) {
                        memcpy(this->as4_path->seg_value,
                                orig.as4_path->seg_value,
                                sizeof (uint32_t) * this->as4_path->length);
                    }
                }
            }
        }

        if (orig.as_path != nullptr) {
            this->as_path = new as_path_segment;
            if (this->as_path != nullptr) {
                this->as_path->seg_type = orig.as_path->seg_type;
                this->as_path->length = orig.as_path->length;
                if (this->as_path->length > 0) {

                    this->as_path->seg_value = new (std::nothrow) uint32_t
                            [this->as_path->length];

                    if (this->as_path->seg_value != nullptr) {
                        memcpy(this->as_path->seg_value,
                                orig.as_path->seg_value,
                                sizeof (uint32_t) * this->as_path->length);
                    }
                }
            }
        }

        if (orig.aggregator != nullptr) {
            this->aggregator = new (std::nothrow) aggregator_segment;
            if (this->aggregator != nullptr) {
                memcpy(this->aggregator, orig.aggregator,
                        sizeof (aggregator_segment));
            }
        }

        if (orig.as4_aggregator != nullptr) {
            this->as4_aggregator = new (std::nothrow) aggregator_segment;
            if (this->as4_aggregator != nullptr) {
                memcpy(this->as4_aggregator, orig.as4_aggregator,
                        sizeof (aggregator_segment));
            }
        }

        if (orig.community != nullptr) {
            if (this->com_seg_length > 0) {
                this->community = new (std::nothrow) community_segment
                        [this->com_seg_length];
                if (this->community != nullptr) {
                    memcpy(this->community, orig.community,
                            sizeof (community_segment) * this->com_seg_length);
                }
            }
        }

        memcpy(&this->nlri, &orig.nlri, sizeof (bgp_nlri));
    }

    void free_all(void) {
        //reset values
        if (this->aggregator != nullptr) {
            delete this->aggregator;
            this->aggregator = nullptr;
        }
        if (this->as4_aggregator != nullptr) {
            delete this->as4_aggregator;
            this->as4_aggregator = nullptr;
        }
        if (this->as4_path != nullptr) {
            delete this->as4_path;
            this->as4_path = nullptr;
        }
        if (this->as_path != nullptr) {
            delete this->as_path;
            this->as_path = nullptr;
        }
        if (this->community != nullptr) {
            delete [] this->community;
            this->community = nullptr;
        }
    }

    // trans. well known. complete.
    uint8_t flags;
    // redis, or network statement
    uint8_t origin;
    // as_path data
    as_path_segment *as_path;
    // as4_path data
    as_path_segment *as4_path;
    // next_hop IP
    uint32_t ipv4_next_hop;
    // metric
    uint32_t med;
    // local AS pref
    uint32_t local_pref;
    // is aggregate missing AS_SET?
    uint8_t atomic_aggregate;
    // aggregator
    aggregator_segment *aggregator;
    // as4_aggregator
    aggregator_segment *as4_aggregator;

    // community segment
    uint8_t com_seg_length;
    community_segment *community;

    // multi-protocol extensions //
    //v4/v6
    uint16_t addr_family;
    //unicast/multicast
    uint8_t addr_family_id;
    // holds next hop length
    uint8_t mp_nlri_nh_len;
    uint8_t subnetwork_points_of_attachment;

    // nlri information
    bgp_nlri nlri;

    bgp_update &operator=(const bgp_update &orig) {
        if (this == &orig) return *this;

        this->free_all();
        this->assign(orig);

        return *this;
    }

    bool operator==(const bgp_update &update) {

        if (update.nlri.prefix == 0)
            return (this->nlri.bgp_id == update.nlri.bgp_id);

        return (
                (!memcmp(&this->ipv4_next_hop,
                &update.ipv4_next_hop, sizeof (uint32_t))
                )&&
                (!memcmp(&this->nlri.prefix,
                &update.nlri.prefix, sizeof (uint32_t)))
                );
    }

    bool operator==(const bgp_nlri &nlri) {
        return (
                (!memcmp(&this->nlri.bgp_id,
                &nlri.bgp_id, sizeof (uint32_t))
                )&&
                (!memcmp(&this->nlri.prefix,
                &nlri.prefix, sizeof (uint32_t)))
                );
    }

    bool operator<(const bgp_update &update) {
        return (
                ((memcmp(&this->nlri.prefix,
                &update.nlri.prefix,
                sizeof (uint32_t)) < 0))
                );
    }

    bool operator<(const bgp_nlri &update) {
        return (
                ((memcmp(&this->nlri.prefix,
                &nlri.prefix,
                sizeof (uint32_t)) < 0))
                );
    }
};

/** Attribute type codes for translation:
 *
 *                Value      Code
 *                  0       PEER BGP-ID
 *                  1       ORIGIN
 *                  2       AS_PATH
 *                  3       NEXT_HOP
 *                  4       MULTI_EXIT_DISC
 *                  5       LOCAL_PREF
 *                  6       ATOMIC_AGGREGATE
 *                  7       AGGREGATOR
 */
struct translate_attribs_base {
    bgp_path_attrib code;
    uint32_t value;
    std::string printable_attrib;
};

extern const char *c_BGP_0_EMERG;
extern const char *c_BGP_1_ALERT;
extern const char *c_BGP_2_CRIT;
extern const char *c_BGP_3_NOTIFICATION;
extern const char *c_BGP_4_WARN;
extern const char *c_BGP_5_NOTICE;
extern const char *c_BGP_5_ADJCHANGE;
extern const char *c_BGP_6_UPDATE;
extern const char *c_BGP_6_WITHDRAW;
extern const char *c_BGP_7_DEBUG;

extern const char *peer_state[];
extern const char *bgp_msg_types[];
extern const char *bgp_error_codes[];
extern const char *bgp_error_sub_header[];
extern const char *bgp_error_sub_open[];
extern const char *bgp_error_sub_update[];
extern const char bgp_rib_code_text[];
extern const char *bgp_origin_code_text[];
#endif	/* MSG_H */

