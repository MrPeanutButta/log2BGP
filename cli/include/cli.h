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

#ifndef _VTYCLI_H_
#define _VTYCLI_H_

#include "cmdparser.h"
using namespace CommandParser;

class CLI : CmdParser {
public:
    CLI();
    CLI(char **);

    ~CLI();

    int Init(void);
    void VTY(void);

    int ParseConfig(void);

protected:

private:
    char environment_vars[255][255];
};

#endif // _VTYCLI_H_
