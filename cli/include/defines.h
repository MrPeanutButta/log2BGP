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

#ifndef _DEFINES_H_
#define _DEFINES_H_

/// Catchable Exceptions.

enum Exception {
    AT_MAX_THREADS, NEG_INDEX, GET_IF_ADDRS_FAIL, TX_SOCKET_OPEN_FAIL,
    SIOCGIFINDEX_ERR, SIOCGIFHWADDR_ERR, SIOCGIFADDR_ERR, TX_ERR,
    RX_SOCKET_OPEN_FAIL, SETSOCKOPT_ERR, SO_BINDTODEVICE_ERR, VTYSH_SOCKET_ERR,
    VTYSH_SOCKET_BIND_ERR, NORMAL_EXIT, NAMED_PIPE_FAIL, VTY_GET_NAME_ERR,
    STARTUP_CONFIG_ERROR, BGP_INIT_ERR, GET_ADDR_INFO
};

#define _DEBUG_DEV_

#endif
