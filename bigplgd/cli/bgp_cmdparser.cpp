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

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "bgp_cmdparser.h"
#include "bgp.h"

/// Contains the command line parsing class.
namespace command_parser {
    /// The command tree.
    std::vector<COMMAND> cmd_parser::commands{};

    /// Pointer to the current COMMAND.
    COMMAND const *cmd_parser::current_token{};

    /// Compare command names

    bool compare_cmd(COMMAND a, COMMAND b) {
        return a.name < b.name;
    }
}

using namespace command_parser;

cmd_parser::cmd_parser() {
}

cmd_parser::~cmd_parser() {

    for (COMMAND &cmd : commands)
        cmd.sub_command.erase(cmd.sub_command.begin(), cmd.sub_command.end());

    commands.erase(commands.begin(), commands.end());

    commands.clear();
}

/** CmdParser::CmdInit - Push and sort the command vectors.
 *
 * Push and sort the command vectors.
 */
void cmd_parser::CmdInit(void) {

    // Keep command struct local scope to not create a leak
#include "bgp_command_struct.h"

    std::vector<COMMAND> *cmd = nullptr;

    commands.clear();

    // Sort main sub command vectors
    for (int i = 0; !cmds[i].name.empty(); i++) {
        commands.emplace_back(cmds[i]);

        if (!commands.back().sub_command.empty()) {
            for (cmd = &commands.back().sub_command; !cmd->empty();
                    cmd = &cmd->back().sub_command) {
                std::sort(cmd->begin(), cmd->end(), compare_cmd);
            }
        }
    }

    // Sort root commands
    std::sort(commands.begin(), commands.end(), compare_cmd);
}

/// Execute a command line.

std::string cmd_parser::ExecuteLine(BGP *bgp, const char *c_line) {

    std::string line(c_line);

    if (line.empty()) return "";

    std::istringstream iss(line);
    iss.clear();

    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string > (iss),
            std::istream_iterator<std::string > (),
            std::back_inserter<std::vector<std::string> >(tokens));

    COMMAND const *cmd = FindCommand(tokens);

    if (!cmd) {
        std::stringstream s_s;
        s_s.clear();
        s_s << c_line <<
                " % Unknown command or computer name, \
                or unable to find computer address.\n";
        tokens.clear();
        return s_s.str().c_str();
    }

    return (*cmd->func)(bgp, tokens);
}

/** CmdParser::FindCommand - Search the command tree for tokens in the
 *                           command line.
 *
 * Look up first token of 'line' as the name of a command, and, if found,
 * assign a pointer 'current_token' to that COMMAND struct. Return a NULL
 * pointer if the token isn't a command name.
 *
 * Continue to process command line tokens until tokens.empty().
 * Final (current_token) * points to the current_token->sub_command[i]
 */
COMMAND const *cmd_parser::FindCommand(std::vector<std::string> tokens) {

    if (tokens.empty()) return (COMMAND*) NULL;

    current_token = (COMMAND*) NULL;
    std::string cmd;

    cmd = tokens.front();
    tokens.erase(tokens.begin());

    for (unsigned int i(0); i < commands.size(); i++) {
        if (!commands[i].name.compare(0, cmd.length(), cmd)) {
            current_token = &commands[i];
        }//if
    }//for

    if (current_token && !tokens.empty()) {
        while (!tokens.empty()) {
            cmd = tokens.front();
            tokens.erase(tokens.begin());
            for (unsigned int i(0); i < current_token->sub_command.size();
                    i++) {
                if (!current_token->sub_command[i].name.compare(0,
                        cmd.length(), cmd) ||
                        current_token->sub_command[i].name[0] == '<') {

                    if (tokens.empty()) {
                        current_token = &current_token->sub_command[i];
                        return current_token;
                    } else if (!tokens.empty()) {
                        current_token = &current_token->sub_command[i];
                        break;
                    }//if
                }//if
            }//for
        }//while
    }//if(current_token && !tokens.empty())

    tokens.clear();
    return current_token;
}