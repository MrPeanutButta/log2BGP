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

#ifndef ADJRIBWDRAWN_H
#define	ADJRIBWDRAWN_H

#include <vector>
#include "bgp_msg.h"

class AdjRibWDrawn : public std::vector<bgp_nlri*> {
public:
    AdjRibWDrawn();
    AdjRibWDrawn(const AdjRibWDrawn& orig);

    virtual ~AdjRibWDrawn();

    int Entry(bgp_nlri &);
    int Flush(uint32_t &bgp_id);

private:

};

#endif	/* ADJRIBWDRAWN_H */

