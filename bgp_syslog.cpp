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

#include <syslog.h>
#include "bgp_syslog.h"
#include "bgp_msg.h"
#include "misc.h"
#include "bgp.h"

bgp_syslog::bgp_syslog(const bgp_syslog& orig) {
}

/** bgp_syslog::translate_and_dump_update - update dumper
 */
void bgp_syslog::translate_and_dump_update(BGP *bgp, const bgp_update &update,
        nlri_parse FLAG) {
    std::stringstream s_s;

    s_s.clear();

    s_s << " *" << time_stamp() << ":" << " flowlab: " << " msg_type=";

    switch (FLAG) {
        case nlri_parse::update:
            s_s << c_BGP_6_UPDATE << "|";
            break;
        case nlri_parse::withdraw:
            s_s << c_BGP_6_WITHDRAW << "|";
            break;
    }

    s_s << "peer=" << bgp->translate_bgp_id(update.nlri.bgp_id) << "|origin="
            << bgp_origin_code_text[update.origin];

    if (update.as_path != nullptr) {
        if (update.as_path->seg_value != nullptr) {
            s_s << "|as_path=";
            for (uint8_t i(0); i < update.as_path->length; ++i) {
                s_s << bgp->translate_as_path(update.nlri.bgp_id,
                        update.as_path->seg_value[i]);
                if ((i + 1) < update.as_path->length)s_s << '_';
            }
            s_s << "|";
        }
    }

    if (update.as4_path != nullptr) {
        if (update.as4_path->seg_value != nullptr) {
            s_s << "as4_path=";
            for (uint8_t i(0); i < update.as4_path->length; ++i) {
                s_s << bgp->translate_as_path(update.nlri.bgp_id,
                        update.as4_path->seg_value[i]);
                if ((i + 1) < update.as4_path->length)s_s << '_';
            }
            s_s << "|";
        }
    }

    s_s << "next_hop=" << bgp->translate_ipv4_next_hop(update.nlri.bgp_id,
            update.ipv4_next_hop)
            << "|med=" << bgp->translate_med(update.nlri.bgp_id, update.med)
            << "|local_pref=" << bgp->translate_local_pref(update.nlri.bgp_id,
            update.local_pref);

    if (update.atomic_aggregate != 0) {
        s_s << "|atomic_aggregate=true|";
    }

    if (update.aggregator != nullptr) {
        s_s << "|aggregator_origin=" <<
                bgp->translate_aggregator(update.nlri.bgp_id,
                update.aggregator->origin)
                << "|aggregator_as=" << bgp->translate_as_path(update.nlri.bgp_id,
                update.aggregator->as);
    }

    if (update.as4_aggregator != nullptr) {
        s_s << "|as4_aggregator_origin=" <<
                bgp->translate_aggregator(update.nlri.bgp_id,
                update.as4_aggregator->origin)
                << "|as4_aggregator_as=" << bgp->translate_as_path(update.nlri.bgp_id,
                update.as4_aggregator->as);
    }

    if (update.community != nullptr) {
        s_s << "|community=";
        for (uint8_t i(0); i < update.com_seg_length; ++i)
            s_s << '[' << bgp->translate_as_path(update.nlri.bgp_id,
                update.community[i].as) << ':'
            << bgp->translate_community_value(update.nlri.bgp_id,
                update.community[i].value) << ']';
    }


    s_s << "|prefix=" << ip_to_string(update.nlri.prefix) << "|prefix_length="
            << (int) update.nlri.prefix_length;

    udp_destinations::const_iterator __it = __udp_map.find(update.nlri.bgp_id);

    if (__it != __udp_map.end()) {
        __udp_map[update.nlri.bgp_id].write(LOG_LOCAL7, LOG_NOTICE, s_s.str());
    } else {
        dumper.open("/var/log/flowlab-updates.log", std::ios::app | std::ios::out);

        if (dumper.is_open()) {
            dumper << s_s.str() << std::endl;
            dumper.close();
        }
    }

    s_s.str().clear();
}

void bgp_syslog::add_dest_host(const uint32_t& ipv4,
        const std::string& dest_host_ipv4, const uint16_t& port) {

    __udp_map[ipv4](ipv4, dest_host_ipv4, port);

}

udp_destinations &bgp_syslog::get_udp_destinations(void) {
    return this->__udp_map;
}


