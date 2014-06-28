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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <sstream>
#include <iterator>
#include <vector>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iomanip>

#ifdef USE_CLI

char *dupstr(int s) {
    char *r;

    r = (char *) malloc(strlen((const char *) s) + 1);
    strcpy(r, (const char *) s);
    return (r);
}

// pipe output to more

int more(const char *buffer) {

    pid_t pid = fork();
    int status = 0;

    if (!pid) {
        FILE * output{};

        output = popen("more", "w");
        if (!output) {
            fprintf(stderr,
                    "incorrect parameters or too many files.\n");
            return EXIT_FAILURE;
        }

        fprintf(output, "%s", buffer);

        if (pclose(output) != 0) {
            fprintf(stderr,
                    "Could not run more or other error.\n");
        }

        exit(EXIT_SUCCESS);

    } else waitpid(pid, &status, WUNTRACED | WCONTINUED);

    return status;
}
#endif

#ifdef USE_BGPD

bool is_stack(void *ptr) {
    pthread_t self = pthread_self();
    pthread_attr_t attr;
    void *stack;
    size_t stacksize;
    pthread_getattr_np(self, &attr);
    pthread_attr_getstack(&attr, &stack, &stacksize);
    return ((uintptr_t) ptr >= (uintptr_t) stack
            && (uintptr_t) ptr < (uintptr_t) stack + stacksize);
}

const char *alloc_output(std::string output) {
    char *buffer = new char [output.length() + 1];

    if (buffer != nullptr) {
        memset(buffer, 0, output.length() + 1);
        memcpy(buffer, output.c_str(), output.length());
    }

    output.clear();
    return buffer;
}

std::string time_stamp(void) {
    time_t local_time;
    char time_string[80];
    time(&local_time);
    strftime(time_string, sizeof (time_string), "%m-%d-%Y %H:%M:%S", gmtime(&local_time));
    return std::string(time_string);
}

std::string time_hh_mm_ss(const time_t &seconds_t) {

    time_t sec_passed = seconds_t;
    int hours = sec_passed / 3600;
    sec_passed %= 3600;
    int mins = sec_passed / 60;
    sec_passed %= 60;
    int secs = sec_passed;

    std::stringstream s_s;
    s_s << std::setfill('0') << std::setw(4) << hours << ":"
            << std::setfill('0') << std::setw(2) << mins << ":"
            << std::setfill('0') << std::setw(2) << secs;

    return s_s.str();
}

std::string int_to_string(const int &value) {
    std::stringstream s_s;
    s_s.clear();
    s_s << value;
    return s_s.str();
}

std::string ip_to_string(const uint32_t &ip) {
    in_addr in;
    in.s_addr = ip;
    return std::string(inet_ntoa(in));
}

// return ip on success, 0 on failure

uint32_t string_to_ip(const std::string &ip) {
    in_addr n_ip;

    if (ip.empty()) return 0;

    if (inet_aton(ip.c_str(), &n_ip) != -1) {
        return n_ip.s_addr;
    }

    perror("string_to_ip");
    return 0;
}

uint8_t string_to_uint8_t(const std::string &value) {
    std::stringstream s_s(value);
    uint8_t ret_val;
    return (s_s >> ret_val) ? ret_val : 0;
}

uint16_t string_to_uint16_t(const std::string &value) {
    std::stringstream s_s(value);
    uint16_t ret_val;
    return (s_s >> ret_val) ? ret_val : 0;
}

uint32_t string_to_uint32_t(const std::string &value) {
    std::stringstream s_s(value);
    uint32_t ret_val;
    return (s_s >> ret_val) ? ret_val : 0;
}
#endif