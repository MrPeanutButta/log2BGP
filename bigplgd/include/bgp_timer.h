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

#ifndef TIMER_H
#define	TIMER_H

#include <thread>

class BGP;
struct bgp_peer;

namespace BGPTimers {
    typedef int (*BGP_func_peer)(BGP *, bgp_peer *);
    typedef int (*BGP_queue_check)(BGP *);
};

class Timer {
public:
    Timer();
    ~Timer();

    void interval(const int &intrval) {
        this->intval = intrval;
    };

    bool BGP_function(void) {
        return this->bgp_function;
    }

    void Start(void);
    void Stop(void);
    void Set(BGP *, BGPTimers::BGP_func_peer, bgp_peer *, const int &);
    void Set(BGP *, BGPTimers::BGP_func_peer, bgp_peer *, const uint16_t &);
    void Set(BGP *, BGPTimers::BGP_queue_check, const int &);

private:

    static void TimerLoop(Timer *);

    bool stop;
    bool bgp_function;

    int intval;

    BGP *bgp;
    bgp_peer *peer;
    BGPTimers::BGP_func_peer func;
    BGPTimers::BGP_queue_check bgp_func;
    std::thread *timer_thread;

};

#endif	/* TIMER_H */

