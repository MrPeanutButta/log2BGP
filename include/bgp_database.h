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

#ifndef DATABASE_H
#define	DATABASE_H

#include <fstream>
#include <string>
#include <deque>
#include <vector>
#include <utility>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>
#include "bgp_msg.h"
#include "bgp_adj_rib.h"

/* index == starting position of fstream
 * where the record can be found. */
typedef std::vector<int> indexes;
typedef std::map<prefix_entry, indexes> db_indexes;

// return all found records
typedef std::vector<bgp_update> records;

class BGP;

class bgp_database {
public:
    bgp_database();
    bgp_database(const bgp_database& orig);
    virtual ~bgp_database();

    // open new data base
    bool db_open(const std::string &);
    // insert entry
    void db_insert(const bgp_update &);
    // get copy of last 10 entries
    records db_get_entries(BGP *, const bgp_update &);

    void db_lock(void) {
        while (!this->db_mutex.try_lock()) {
            std::this_thread::sleep_for(
                    std::chrono::nanoseconds(5)
                    );
        }
    }

    void db_unlock(void) {
        this->db_mutex.unlock();
    }

    void max_records(uint8_t max) {
        max_records_ = max;
    }

private:
    // open existing database
    bool db_open(void);

    std::mutex db_mutex;
    std::fstream dbase;
    // database name
    std::string db_name;
    // index mapper
    db_indexes index_map;

    uint16_t max_records_;
};

// database markers.
// Path attribute codes.

namespace dbase_attrib {
    const uint32_t origin = 0xFFFFFF01,
            as_path = 0xFFFFFF02,
            next_hop = 0xFFFFFF03,
            multi_exit_disc = 0xFFFFFF04,
            local_pref = 0xFFFFFF05,
            atomic_aggregate = 0xFFFFFF06,
            aggregator = 0xFFFFFF07,
            community = 0xFFFFFF08,
            originator = 0xFFFFFF09,
            cluster_list = 0xFFFFFF0A,
            mp_reach_nlri = 0xFFFFFF0E,
            mp_unreach_nlri = 0xFFFFFF0F,
            as4_path = 0xFFFFFF11,
            as4_aggregator = 0xFFFFFF12,
            addr_family = 0xFFFFFF13,
            addr_family_id = 0xFFFFFF14,
            nlri = 0xFFFFFF15,
            end_of_record = 0xFFFFFFFF;
};

#endif	/* DATABASE_H */

