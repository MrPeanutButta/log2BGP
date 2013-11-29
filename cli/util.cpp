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

#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <string>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pty.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include "cmdparser.h"
#include "cli.h"
#include "util.h"
#include "misc.h"
#include "tcp_client.h"

extern bool config_loader;

void Util::InitProcPipes(void) {
    init_bgp_proc_pipe();
}

int Util::WriteConfigs(std::vector<std::string> &tokens) {
    using namespace Utility;

    //std::cout << "Building configurations...";

    if (bgp->connected()) {
        write_to_bgp(std::string("write"));
        read_from_bgp();
    }



    tokens.clear();
    return 0;
}

int Util::HostName(std::vector<std::string> &tokens) {

    if (tokens.size() < 2) {
        std::cerr << "hostname: requires argument 'hostname'\n";
        tokens.clear();
        return -1;
    }

    if (tokens.size() > 2) {
        std::cerr << "hostname: too many arguments\n";
        tokens.clear();
        return -1;
    }

    if (sethostname(tokens.at(1).c_str(), tokens.at(1).length())) {
        std::cerr << "hostname: unable to set hostname to " << tokens.at(1) << std::endl;
        tokens.clear();
        return -1;
    }

    p_s.PSInit();
    tokens.clear();
    return 0;
}

int Util::Enable(std::vector<std::string> &tokens) {
    using namespace Utility;
    using namespace std;

    if (!config_loader) char *enable_pass = getpass("Password: ");

    p_s.ExecPrompt();
    tokens.clear();
    return 0;
}

int Util::End(std::vector<std::string> &tokens) {
    p_s.ExecPrompt();
    tokens.clear();
    return 0;
}

int Util::Exit(std::vector<std::string> &tokens) {
    p_s.ExitOneLevel();
    tokens.clear();
    return 0;
}

int Util::Proto(std::vector<std::string> &tokens) {

    if (tokens.size() < 2) {
        std::cerr << "protocol: requires argument 'process'\n";
        tokens.clear();
        return -1;
    }

    if (!tokens.at(1).compare(0, tokens.at(1).length(), "bgp", 0, tokens.at(1).length()))
        return ProtoBGP(tokens);

    tokens.clear();

    return 0;
}

int Util::Configure(std::vector<std::string> &tokens) {

    if (tokens.size() < 2) {
        std::cerr << "configure: requires an argument\n";
        tokens.clear();
        return -1;
    }

    if (tokens.size() > 2) {
        std::cerr << "configure: too many arguments\n";
        tokens.clear();
        return -1;
    }

    if (!tokens.at(1).compare(0, tokens.at(1).length(), "terminal", 0, tokens.at(1).length()))
        return ConfigureTerminal(tokens);

    if (!tokens.at(1).compare(0, tokens.at(1).length(), "sub-system", 0, tokens.at(1).length()))
        return ConfigureSubSystem(tokens);

    std::cerr << "configure: invalid argument\n";
    tokens.clear();

    return -1;
}

int Util::ConfigureTerminal(std::vector<std::string> &tokens) {
    p_s.EnterConfig();
    tokens.clear();

    return 0;
}

int Util::ConfigureSubSystem(std::vector<std::string> &tokens) {
    tokens.clear();

    return system("sudo /bin/bash");
}

int Util::Disable(std::vector<std::string> &tokens) {
    p_s.UserPrompt();
    tokens.clear();

    return 0;
}

int Util::Show(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::CD(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::PWD(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::Delete(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::Rename(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::List(std::vector<std::string> &tokens) {
    std::cout << "list\n";
    tokens.clear();

    return 0;
}

int Util::Ping(std::vector<std::string> &tokens) {

    if (tokens.size() < 2) {
        std::cerr << "ping: requires an argument\n";
        tokens.clear();
        return -1;
    }

    std::string ping = "ping -c 1 -i 0.2 " + tokens[1] + " > /dev/null 2>&1";

    for (int i = 0; i < 5; i++) {
        if (!system(ping.c_str())) {
            std::cout << "!";
        } else {
            std::cout << ".";
        }
    }

    std::cout << std::endl;

    tokens.clear();

    return 0;
}

int Util::Stat(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

int Util::Help(std::vector<std::string> &tokens) {
    tokens.clear();

    return 0;
}

/*!Attempt to provide command line help.
 *
 * Checks for available command line options when '?' is pressed
 */
int Util::QMark(std::vector<std::string> &dmy_tokens) {
    using namespace std;

    cout << endl;

    if (rl_line_buffer[0] == '\0') {
        QMarkDisplayAllInMode();
        std::cout << p_s.PS();
        rl_on_new_line_with_prompt();
        return 0;
    }

    istringstream iss(rl_line_buffer);
    iss.clear();

    vector<string> tokens;
    copy(istream_iterator<string > (iss), istream_iterator<string > (), back_inserter<vector<string> >(tokens));

    COMMAND *cmd = FindCommand(tokens);

    if (cmd && !cmd->sub_command.empty()) {
        if (!isspace(rl_line_buffer[strlen(rl_line_buffer) - 1]))rl_insert_text(" ");
        for (int i = 0; i < cmd->sub_command.size(); i++) {
            QMarkDisplayAvailable(cmd->sub_command[i].name, cmd->sub_command[i].doc);
        }
    } else if (cmd && cmd->sub_command.empty()) {
        QMarkDisplayAvailable(cmd->name, cmd->doc);
    }

    cout << p_s.PS();
    rl_on_new_line_with_prompt();

    return 0;
}

void Util::QMarkDisplayAvailable(std::string &c, std::string & d) {
    using namespace std;

    if (c.length() >= 6) {
        cerr << "  " << c << "\t\t\t" << d << endl;
    } else {

        cerr << "  " << c << "\t\t\t\t" << d << endl;
    }
}

void Util::QMarkDisplayAvailable(const std::string &c, const std::string & d) {
    using namespace std;

    if (c.length() >= 6) {
        cerr << "  " << c << "\t\t\t" << d << endl;
    } else {

        cerr << "  " << c << "\t\t\t\t" << d << endl;
    }
}

void Util::QMarkDisplayAllInMode(void) {
    using namespace std;

    for (int i = 0; i < commands.size(); i++) {
        if ((p_s.Mode() & commands[i].mode)) {
            if (commands[i].name.length() >= 6) {
                cerr << "  " << commands[i].name << "\t\t\t" << commands[i].doc << endl;
            } else {
                cerr << "  " << commands[i].name << "\t\t\t\t" << commands[i].doc << endl;
            }
        }//if
    }//for
}