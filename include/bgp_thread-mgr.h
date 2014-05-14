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

#ifndef _THREAD_MGR_H_
#define _THREAD_MGR_H_

#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <memory>
#include "bgp_peer.h"

class BGP;

typedef std::shared_ptr<bgp_peer> peer_t;

class thread_mgr {
public:

    explicit thread_mgr();
    virtual ~thread_mgr() = 0;

    virtual void start_peer(BGP *, bgp_peer *);

    virtual int delete_peer(bgp_peer *);

    virtual void TLog(std::string);

    virtual int peer_count() {
        return thread_q.size();
    }

    virtual bgp_peer *thread_base_peer_at(int index) {
        return thread_q.at(index).get();
    }

    virtual bool peer_exists(const bgp_peer *);
    virtual bool peer_exists(const std::string &);
    virtual bool peer_doing_alias(const uint32_t &);
    virtual bgp_peer *get_peer_by_str_ip(const std::string &);
    bgp_peer *get_peer_by_uint32_t_bgp_id(const uint32_t &);

protected:

private:

    friend BGP;
    // neighbor table
    std::vector<peer_t> thread_q;
};

#endif // _THREAD_MGR_H_
