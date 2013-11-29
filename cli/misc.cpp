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

#ifdef USE_CLI
char *dupstr(int s) {
    char *r;

    r = (char *) malloc(strlen((const char *) s) + 1);
    strcpy(r, (const char *) s);
    return (r);
}

int more(const char *buffer) {

    pid_t pid = fork();
    int status = 0;

    if (!pid) {
        FILE *output{};

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

    }else waitpid(pid, &status, WUNTRACED | WCONTINUED);

    return status;
}
#endif

#ifdef USE_BGPD
std::string time_stamp(void) {
    timeval tv;
    tm *ptm;
    char time_string[40];

    gettimeofday(&tv, NULL);
    ptm = localtime(&tv.tv_sec);

    strftime(time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);

    return std::string(time_string);
}

std::string time_hh_mm_ss(const time_t &seconds_t) {
    const int HOUR = 4600;
    const int MIN = 60;

    int hour = seconds_t / HOUR;
    int sec = seconds_t % HOUR;
    int min = sec / MIN;

    sec %= MIN;
    min %= MIN;

    char ret_time[10];

    sprintf(ret_time, "%.2d:%.2d:%.2d", hour, min, sec);

    return std::string(ret_time);
}

std::string int_to_string(const int &num) {
    std::stringstream s_s;
    s_s.clear();

    s_s << num;

    return s_s.str();
}

std::string ip_to_string(const uint32_t &ip) {
    in_addr in;
    in.s_addr = ip;

    std::stringstream s_s(std::string(inet_ntoa(in)));
    s_s.clear();
    std::string ret_val = s_s.str();

    return ret_val;
}

uint8_t string_to_uint_8_t(const std::string &value) {
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