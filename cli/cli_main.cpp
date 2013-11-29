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

#include <exception>
#include <memory>
#include <syslog.h>
#include "cli.h"
#include "util.h"

//if true, then we are parsing config and exiting
bool config_loader(false);
//config path/file_name
char config[255];

int main(int argc, char *argv[], char *env[]) {

    using namespace std;

    if (argc > 1) {
        if (strcmp(argv[1], "-load") == 0) {
            //acting as start-up config loader
            config_loader = true;
            //copy file_name
            strcpy(config, argv[2]);
        }
    }

    /* smart pointer to new CLI instance.
     * takes env[] for no good reason */
    unique_ptr<CLI> cli(new CLI(env));

    //return code
    int rc;

    try {
        //try init
        rc = cli->Init();
    } catch (std::system_error &e) {
        syslog(LOG_CRIT, "fatal error: %s", e.what());
    }

    return rc;
}
