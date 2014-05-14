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

#ifndef TCP_SERVER_H
#define	TCP_SERVER_H
#include <thread>
#include <vector>
#include "tcp_client.h"

typedef std::vector<std::thread *> connection_threads;

/* function pointer to call to handle the
 * actual connection. either is API or CLI */
typedef void (*connection)(std::thread *, const int);

class tcp_server : public tcp_client {
public:

    tcp_server();
    tcp_server(const tcp_server& orig);
    virtual ~tcp_server() = 0;

    bool listen_for_cli(const char *, const char *);
    bool listen_for_api(const char *, const char *);

    static connection_threads server_connections;

private:

    std::thread *server;

    void bind(void);
    static void listen_loop(const int &, addrinfo, connection con);
    static void connection_loop(std::thread *, const int);
    static void sinatra_api(std::thread *, const int);
};

#endif	/* TCP_SERVER_H */

