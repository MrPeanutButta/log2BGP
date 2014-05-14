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

#include <algorithm>
#include <cstring>
#include <sstream>
#include "bgp_database.h"
#include "bgp_msg.h"
#include "bgp.h"

bgp_database::bgp_database() :
/** max number of entries increased from only 10.
 * this should be configurable */
max_records_(1000) {
}

bgp_database::bgp_database(const bgp_database& orig) {
}

bgp_database::~bgp_database() {
    this->dbase.close();
}

// open new database.

bool bgp_database::db_open(const std::string &db_name) {

    if (!db_name.empty()) {
        this->db_name = db_name;

        /* need to create the file
         * if it doesn't exist. */
        this->dbase.open(this->db_name, std::ios::out);
        this->dbase.close();

        this->dbase.open(this->db_name,
                std::fstream::in |
                std::fstream::out |
                std::fstream::binary);

        return this->dbase.is_open();
    }

    return false;
}

// open existing database

bool bgp_database::db_open(void) {

    if (!this->db_name.empty()) {

        /* need to create the file
         * if it doesn't exist. */
        this->dbase.open(this->db_name, std::ios::out);
        this->dbase.close();

        this->dbase.open(this->db_name,
                std::fstream::in |
                std::fstream::out |
                std::fstream::binary);

        return this->dbase.is_open();
    }

    return false;
}

// insert new entry and record index.

void bgp_database::db_insert(const bgp_update &update) {

    if (!this->dbase.is_open())return;

    /* make index and save position.
     * no need to sort as new inserts are placed
     * at eof. therefor positions are 'self-sorting' */
    this->db_lock();

    indexes *position_vector = &this->index_map[
            std::make_pair(update.nlri.prefix, update.nlri.bgp_id)
            ];

    position_vector->emplace_back((int) this->dbase.tellp());

    // keep only last 10 records
    if (position_vector->size() > max_records_) {
        position_vector->erase(position_vector->begin());
    }

    /* write attribute flags followed by attributes.
     * will be easier to ID which attribute follows
     * next for reading */

    // write addr fam AFI
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::addr_family), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.addr_family), sizeof (uint16_t));

    // write addr fam id SAFI
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::addr_family_id), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.addr_family_id), sizeof (uint8_t));

    // write 2 byte as aggr
    if (update.aggregator != nullptr) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::aggregator), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.aggregator->as), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.aggregator->origin), sizeof (uint32_t));
    }

    // write 4 byte as aggr
    if (update.as4_aggregator != nullptr) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::as4_aggregator), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as4_aggregator->as), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as4_aggregator->origin), sizeof (uint32_t));
    }

    // write 4 byte as path
    if (update.as4_path != nullptr) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::as4_path), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as4_path->length), sizeof (uint8_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as4_path->seg_type), sizeof (uint8_t));

        for (uint8_t i = 0; i < update.as4_path->length; ++i) {
            this->dbase.write(reinterpret_cast<const char *>
                    (&update.as4_path->seg_value[i]),
                    sizeof (uint32_t));
        }
    }

    // write 2 byte as path
    if (update.as_path != nullptr) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::as_path), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as_path->length), sizeof (uint8_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.as_path->seg_type), sizeof (uint8_t));

        for (uint8_t i = 0; i < update.as_path->length; ++i) {
            this->dbase.write(reinterpret_cast<const char *>
                    (&update.as_path->seg_value[i]),
                    sizeof (uint32_t));
        }
    }

    // atomic aggr?
    if (update.atomic_aggregate) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::atomic_aggregate), sizeof (uint32_t));
    }

    // write community
    if (update.community != nullptr) {
        this->dbase.write(reinterpret_cast<const char *>
                (&dbase_attrib::community), sizeof (uint32_t));
        this->dbase.write(reinterpret_cast<const char *>
                (&update.com_seg_length), sizeof (uint8_t));

        for (uint8_t i = 0; i < update.com_seg_length; ++i) {
            this->dbase.write(reinterpret_cast<const char *>
                    (&update.community[i].as), sizeof (uint32_t));
            this->dbase.write(reinterpret_cast<const char *>
                    (&update.community[i].value), sizeof (uint32_t));
        }
    }

    // write ipv4 next hop
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::next_hop), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.ipv4_next_hop), sizeof (uint32_t));

    // write local pref
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::local_pref), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.local_pref), sizeof (uint32_t));

    // write med
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::multi_exit_disc), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.med), sizeof (uint32_t));

    // write nlri info
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::nlri), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.age), sizeof (uint64_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.bgp_id), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.last_withdraw_time), sizeof (uint64_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.prefix), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.prefix_highest), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.prefix_length), sizeof (uint8_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.received), sizeof (uint16_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.subnet_mask), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.nlri.withdrawn), sizeof (uint16_t));

    // write origin
    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::origin), sizeof (uint32_t));
    this->dbase.write(reinterpret_cast<const char *>
            (&update.origin), sizeof (uint8_t));

    this->dbase.write(reinterpret_cast<const char *>
            (&dbase_attrib::end_of_record), sizeof (uint32_t));

    this->dbase.flush();
    this->dbase.sync();
    this->db_unlock();

}

records bgp_database::db_get_entries(BGP *bgp, const bgp_update &entry) {

    this->db_lock();

    // return records
    records ret_rec;

    // attrib code
    uint32_t code = 0;

    // result
    bgp_update *ud_data = nullptr;
    indexes::reverse_iterator r_it;

    for (r_it = index_map[std::make_pair(entry.nlri.prefix,
            entry.nlri.bgp_id)].rbegin();
            r_it != index_map[std::make_pair(entry.nlri.prefix,
            entry.nlri.bgp_id)].rend(); ++r_it) {

        this->dbase.clear();
        // set position
        this->dbase.seekg(*r_it);

        code = 0;
        ud_data = new bgp_update;

        while (code != dbase_attrib::end_of_record) {
            if (code == dbase_attrib::end_of_record) break;

            this->dbase.sync();
            this->dbase.read((char *) &code, sizeof (uint32_t));

            switch (code) {
                case dbase_attrib::origin:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->origin), sizeof (uint8_t));
                    this->dbase.sync();
                    break;
                case dbase_attrib::as_path:

                    ud_data->as_path = new (std::nothrow) as_path_segment;
                    if (ud_data->as_path == nullptr) break;

                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as_path->length),
                            sizeof (uint8_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as_path->seg_type),
                            sizeof (uint8_t));

                    ud_data->as_path->seg_value = new (std::nothrow) uint32_t
                            [ud_data->as_path->length];

                    if (ud_data->as_path->seg_value == nullptr) break;

                    for (uint8_t i = 0; i < ud_data->as_path->length; ++i) {
                        this->dbase.read(reinterpret_cast<char *>
                                (&ud_data->as_path->seg_value[i]),
                                sizeof (uint32_t));
                    }
                    break;
                case dbase_attrib::next_hop:

                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->ipv4_next_hop),
                            sizeof (uint32_t));
                    break;
                case dbase_attrib::multi_exit_disc:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->med), sizeof (uint32_t));
                    break;
                case dbase_attrib::local_pref:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->local_pref), sizeof (uint32_t));

                    break;
                case dbase_attrib::atomic_aggregate:
                    ud_data->atomic_aggregate = 1;
                    break;
                case dbase_attrib::aggregator:

                    ud_data->aggregator = new aggregator_segment;
                    if (ud_data->aggregator == nullptr) break;

                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->aggregator->as),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->aggregator->origin),
                            sizeof (uint32_t));

                    break;
                case dbase_attrib::community:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->com_seg_length),
                            sizeof (uint8_t));

                    ud_data->community = new community_segment
                            [ud_data->com_seg_length];

                    for (int i = 0; i < ud_data->com_seg_length; ++i) {
                        this->dbase.read(reinterpret_cast<char *>
                                (&ud_data->community[i].as),
                                sizeof (uint32_t));
                        this->dbase.read(reinterpret_cast<char *>
                                (&ud_data->community[i].value),
                                sizeof (uint32_t));
                    }

                    break;
                case dbase_attrib::originator:
                    break;
                case dbase_attrib::cluster_list:
                    break;
                case dbase_attrib::mp_reach_nlri:
                    break;
                case dbase_attrib::mp_unreach_nlri:
                    break;
                case dbase_attrib::as4_path:

                    ud_data->as4_path = new (std::nothrow) as_path_segment;

                    if (ud_data->as4_path == nullptr) break;

                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as4_path->length),
                            sizeof (uint8_t));

                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as4_path->seg_type),
                            sizeof (uint8_t));

                    ud_data->as4_path->seg_value = new uint32_t
                            [ud_data->as4_path->length];

                    for (uint8_t i = 0; i < ud_data->as4_path->length; ++i) {
                        this->dbase.read(reinterpret_cast<char *>
                                (&ud_data->as4_path->seg_value[i]),
                                sizeof (uint32_t));
                    }
                    break;
                case dbase_attrib::as4_aggregator:
                    ud_data->as4_aggregator = new aggregator_segment;
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as4_aggregator->as),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->as4_aggregator->origin),
                            sizeof (uint32_t));
                    break;
                case dbase_attrib::addr_family:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->addr_family),
                            sizeof (uint16_t));
                    break;
                case dbase_attrib::addr_family_id:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->addr_family_id),
                            sizeof (uint8_t));
                    break;
                case dbase_attrib::nlri:
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.age), sizeof (time_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.bgp_id),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.last_withdraw_time),
                            sizeof (time_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.prefix),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.prefix_highest),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.prefix_length),
                            sizeof (uint8_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.received),
                            sizeof (uint16_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.subnet_mask),
                            sizeof (uint32_t));
                    this->dbase.read(reinterpret_cast<char *>
                            (&ud_data->nlri.withdrawn),
                            sizeof (uint16_t));
                    break;
                case dbase_attrib::end_of_record:
                    break;
                default:
                    break;
            }
        }

        ret_rec.emplace_back(*ud_data);

        delete ud_data;
        ud_data = nullptr;

    }

    if (ud_data != nullptr) {
        delete ud_data;
        ud_data = nullptr;
    }

    this->db_unlock();

    return ret_rec;
}
