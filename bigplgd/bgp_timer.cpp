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


#include <chrono>
#include <thread>
#include "bgp.h"
#include "bgp_peer.h"
#include "bgp_timer.h"

/** Timer::Set - sets timer params.
 */
void Timer::Set(BGP *bgp, BGPTimers::BGP_func_peer bgp_func,
        bgp_peer *peer, const int &interval) {
    Timer();

    this->bgp_function = false;
    this->bgp = bgp;
    this->func = bgp_func;
    this->peer = peer;
    this->intval = interval;
    this->timer_thread = new std::thread(&Timer::TimerLoop, this);
}

/** Timer::Set - sets timer params.
 */
void Timer::Set(BGP *bgp, BGPTimers::BGP_func_peer bgp_func,
        bgp_peer *peer, const uint16_t &interval) {
    Timer();

    this->bgp_function = false;
    this->bgp = bgp;
    this->func = bgp_func;
    this->peer = peer;
    this->intval = (int) interval;
    this->timer_thread = new std::thread(&Timer::TimerLoop, this);
}

/** Timer::Set - sets timer params
 */
void Timer::Set(BGP *bgp, BGPTimers::BGP_queue_check bgp_func,
        const int &interval) {
    Timer();

    this->bgp_function = true;
    this->bgp = bgp;
    this->intval = interval;
    this->bgp_func = bgp_func;
    this->timer_thread = new std::thread(&Timer::TimerLoop, this);
}

/** Timer::Start - detaches timer thread
 */
void Timer::Start(void) {
    this->stop = false;
    if (this->timer_thread) this->timer_thread->detach();
}

void Timer::Stop(void) {
    this->stop = true;
}

/** Timer::TimerLoop - executes a function on an interval.
 */
void Timer::TimerLoop(Timer *tmr) {

    while (!tmr->stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tmr->intval));

        if (tmr->BGP_function()) {
            (*tmr->bgp_func)(tmr->bgp);
        } else {
            (*tmr->func)(tmr->bgp, tmr->peer);
        }
    }

    delete tmr->timer_thread;
}

Timer::Timer() : stop(false), bgp_function(false), intval(0) {
    timer_thread = nullptr;
    this->bgp = nullptr;
    this->bgp_func = nullptr;
    this->func = nullptr;
    this->peer = nullptr;
}

Timer::~Timer() {
    if (timer_thread != nullptr) delete this->timer_thread;

    timer_thread = nullptr;
    this->bgp = nullptr;
    this->bgp_func = nullptr;
    this->func = nullptr;
    this->peer = nullptr;
}

