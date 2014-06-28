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


#ifndef BGP_ADJ_RIB_H

#include <utility>

// namespace std;

/** RIB indexing by prefix, bgp_id pair.
 */
typedef std::pair<uint32_t, uint32_t> prefix_entry;

struct rib_sort {
    // returns lexicographical ordering. bgp_id breaks tie

    bool operator()(const prefix_entry &a, const prefix_entry &b) {

        return (memcmp(&a.first, &b.first, sizeof (uint32_t)) < 0) ||
                (!memcmp(&a.first, &b.first, sizeof (uint32_t)) &&
                (memcmp(&a.second, &b.second, sizeof (uint32_t)) < 0));
    }
};

/** RIB typedef.
 *
 * sorted unique entries via 'prefix_entry'.
 */
typedef std::map<prefix_entry, bgp_update, rib_sort> bgp_adj_rib;

#define	BGP_ADJ_RIB_H



#endif	/* BGP_ADJ_RIB_H */

