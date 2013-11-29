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

#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "cli.h"
#include <readline/readline.h>

extern bool config_loader;
extern char config[];

/*! CLI entry point.
 */
int CLI::Init(void) {

    //init command tree
    CmdInit();

    //if true, parse start-up config
    if (config_loader) {
        //just parse and exit
        return ParseConfig();
    } else {
        //read commands from terminal
        VTY();
    }

    //always 0?
    return 0;
}

int CLI::ParseConfig(void) {

    //return code
    int rc;

    //enter enable mode
    rc = ExecuteLine("enable");

    if (rc < 0) {
        std::cerr << "exec error: unable to enter priv exec mode\n";
        return -1;
    }

    //enter configuration mode
    rc = ExecuteLine("configure terminal");

    if (rc < 0) {
        std::cerr << "exec error: unable to enter configure mode\n";
        return -1;
    }

    std::ifstream startup_config;

    //open config file
    startup_config.open(config, std::ios::in);

    if (startup_config.fail()) {
        startup_config.close();
        std::cerr << "config error: unable to open startup-config [ " << config << " ]\n";
        return -1;
    }

    //input buffer
    char buffer[255];

    //readline until EOF
    while (!startup_config.eof()) {
        memset(&buffer, 0, sizeof (buffer));
        startup_config.getline(buffer, 255, '\n');

        //execute config statement
        rc = ExecuteLine(buffer);

        //report any errors
        if (rc < 0) {
            std::cerr << "config error: [ " << buffer << " ]\n";
            startup_config.close();
            return -1;
        }

        /* pause a bit before proceeding
         * to allow any daemons to spin-up
         * before attempting to write to a broken
         * socket */
        usleep(1000);
    }

    startup_config.close();

    //exit with status
    std::cout << "Parsing complete on [" << config << " ]\n";
    std::cout << "Prompt = " << p_s.PS() << std::endl;

    //always 0?
    return 0;
}

void CLI::VTY(void) {

    char *line = nullptr;

    // Loop reading and executing lines until the user quits.
    for (;;) {
        rl_bind_key('\t', rl_complete);

        line = readline(p_s.PS());
        //attempt to execute line
        if (line)ExecuteLine(line);

        free(line);
        line = nullptr;
    }
}

CLI::CLI(char **env) {
    memcpy(environment_vars, env, sizeof (env));
}

CLI::CLI() {

}

CLI::~CLI() {

}

