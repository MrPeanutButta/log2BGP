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

#ifndef VTY_MISC_H
#define VTY_MISC_H

#include <string>

#ifdef USE_CLI
extern char *dupstr(int);
extern pid_t proc_found(const char*);
extern int more(const char *);
#endif

#ifdef USE_BGPD
extern std::string time_hh_mm_ss(const time_t &);
extern std::string int_to_string(const int &);
extern uint8_t string_to_uint8_t(const std::string &);
extern uint32_t string_to_uint32_t(const std::string &);
extern std::string time_stamp(void);
extern std::string ip_to_string(const uint32_t &);
extern uint16_t string_to_uint16_t(const std::string &);
#endif

#endif
