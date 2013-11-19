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

/* this CM has become deprecated because of the
 * use of bgp_database */

#include "bgp_adj_rib_wdrawn.h"

AdjRibWDrawn::AdjRibWDrawn() {
}

AdjRibWDrawn::AdjRibWDrawn(const AdjRibWDrawn& orig) {
}

AdjRibWDrawn::~AdjRibWDrawn() {
}

/** AdjRibWDrawn::Entry - deprecated
 */
int AdjRibWDrawn::Entry(bgp_nlri &nlri) {
    bool found = false;
    for (unsigned int i(0); i < this->size(); i++) {
        if (this->at(i)->bgp_id == nlri.bgp_id) {
            if (this->at(i)->prefix == nlri.prefix) {
                this->at(i)->withdrawn++;
                this->at(i)->last_withdraw_time = nlri.last_withdraw_time;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        nlri.withdrawn++;
        this->push_back(new bgp_nlri(nlri));
    }

    return 0;
}

/** AdjRibWDrawn::Flush - Empty updates from peer due to clear/down session
 */
int AdjRibWDrawn::Flush(uint32_t &bgp_id) {

    for (unsigned int i(0); i < this->size(); i++) {
        if (this->at(i)->bgp_id == bgp_id) {
            this->erase(this->begin() + i);
            break;
        }
    }

    return 0;
}
