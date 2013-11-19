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

#include <exception>
#include <memory>
#include <fcntl.h>
#include <syslog.h>
#include "bgp.h"

// instance check via lock file
bool another_instance(void);
// locked file to prevent multiple instances
int lock_file(0);

int main(int argc, char *argv[]) {

    if (argc < 2)return -1;
    if (another_instance()) return -179;

#ifndef BGP_GDB_DEBUG_NO_DAEMON
    // try to daemon
    if (daemon(0, 1)) return errno;
#endif

    try {
        // open syslog as BGP for debug
        openlog("BGP", LOG_CONS, LOG_LOCAL7);
        std::auto_ptr<BGP> bgp(new BGP);
        // get 2 byte AS from argv[1]
        uint16_t AS((uint16_t) atoi(argv[1]));
        if (AS)bgp->init_BGP(AS);
        else {
            syslog(LOG_DEBUG, "Invalid AS=%i", AS);
        }
        // catch all system errors
    } catch (std::system_error &e) {
        syslog(LOG_CRIT, "fatal error: %s", e.what());
    }

    close(lock_file);
    remove("/tmp/bgp_lock");
    return 0;
}

/** Checks lock for /tmp/bgp_lock
 *
 * Checks /tmp/bgp_lock, if locked then there is already an instance running
 */
bool another_instance() {

    struct flock fl;

    lock_file = open("/tmp/bgp_lock", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    if (lock_file == -1) {
        syslog(LOG_DEBUG, "unable to open /tmp/bgp_lock");
        return true;
    }

    fl.l_type = F_WRLCK; // F_RDLCK, F_WRLCK, F_UNLCK
    fl.l_whence = SEEK_SET; // SEEK_SET, SEEK_CUR, SEEK_END
    fl.l_start = 0; // offset from l_whence
    fl.l_len = 0; // length, 0 = to EOF
    fl.l_pid = getpid(); // our PID

    // try to create a file lock

    // F_GETLK, F_SETLK, F_SETLKW
    if (fcntl(lock_file, F_SETLK, &fl) == -1) {

        // we failed to create a file lock, meaning it's already locked.
        if (errno == EACCES || errno == EAGAIN) {
            syslog(LOG_DEBUG, "unable to aquire lock on /tmp/bgp_lock");
            return true;
        }
    }

    return false;
}