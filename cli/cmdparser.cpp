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
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "cmdparser.h"
#include "util.h"
#include "misc.h"
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

/// Contains the command line parsing class.
namespace CommandParser {
    /// The command tree.
    vector<COMMAND> CmdParser::commands;
    //static int CmdParser::psuedo_term;
    string CmdParser::psuedo_term_in;
    string CmdParser::psuedo_term_out;
    prompt CmdParser::p_s;

    /// Pointer to the current COMMAND.
    COMMAND const *CmdParser::current_token = NULL;

    /// Compare command names

    bool compare_cmd(COMMAND a, COMMAND b) {
        return a.name < b.name;
    }
}

CmdParser::CmdParser(void) {
}

CmdParser::~CmdParser(void) {
}

/*!Push and sort the command vectors.
 *
 *Push and sort the command vectors.
 *
 *Init readline settings.
 */
void CmdParser::CmdInit(void) {
    /// File containing the command struct.
    using namespace CommandParser;

    // Keep command struct seperate for ease of editing
#include "command_struct.h"

    std::vector<COMMAND> *cmd = nullptr;

    // Sort main sub command vectors
    for (int i = 0; !cmds[i].name.empty(); i++) {
        commands.push_back(cmds[i]);

        if (!commands.back().sub_command.empty()) {
            for (cmd = &commands.back().sub_command; !cmd->empty(); cmd = &cmd->back().sub_command) {
                std::sort(cmd->begin(), cmd->end(), compare_cmd);
            }
        }
    }

    // Sort root commands
    std::sort(commands.begin(), commands.end(), compare_cmd);

    Util::InitProcPipes();
    InitReadLine();

    p_s.PSInit();
    p_s.UserPrompt();

}

char *CmdParser::CommandCompletionEntry(const char *ignore0, int invoking_key) {
    return nullptr;
}

/*!Init GNU Readline library.
 *
 * We want to try to complete on command names if this is the first word, or subsequent
 * token in the line, or some user defined name if not.
 */
void CmdParser::InitReadLine(void) {

    rl_terminal_name = "VT100";

    // Bind '?' key to com_q_mark
    rl_bind_key(0x3f, Util::QMark);

    rl_completion_entry_function = CmdParser::CommandCompletionEntry;
    // Tell the completer that we want a crack first at '\t'.
    rl_attempted_completion_function = CmdParser::CommandCompletion;
    rl_initialize();
}

void strip_comment(char *c_line) {
    for (int i = 0; i < strlen(c_line); i++) {
        if (c_line[i] == '!') {
            c_line[i] = '\0';
            return;
        }
    }
}

/// Execute a command line.

int CmdParser::ExecuteLine(char *c_line) {

    strip_comment(c_line);
    string line(c_line);
    line = trim_copy(line);

    if (line.empty()) return 0;

    istringstream iss(line);
    iss.clear();

    vector<string> tokens;
    copy(istream_iterator<string > (iss),
            istream_iterator<string > (),
            back_inserter<vector<string> >(tokens));

    COMMAND *cmd = FindCommand(tokens, DupChk);

    if (cmd == nullptr) {
        cerr << "unknown command: " << c_line << endl;
        tokens.clear();
        return -1;
    }

    if (c_line && *c_line) {
        using_history();
        HIST_ENTRY *last = previous_history();
        if (!last || strcmp(last->line, c_line) != 0) {
            add_history(line.c_str());
        }
    }

    return (*cmd->func)(tokens);
}

/*!Search the command tree for tokens in the command line.
 *
 * Look up first token of 'line' as the name of a command, and, if found,
 * assign a pointer 'current_token' to that COMMAND struct. Return a NULL
 * pointer if the token isn't a command name.
 *
 * Continue to process command line tokens until tokens.empty(). Final (current_token)
 * points to the current_token->sub_command[i]
 */
COMMAND *CmdParser::FindCommand(std::vector<std::string> tokens, bool dup_chk) {

    if (tokens.empty()) return nullptr;

    current_token = nullptr;

    string cmd;

    cmd = tokens.front();
    tokens.erase(tokens.begin());

    for (int i = 0; i < commands.size(); ++i) {
        if (!commands[i].name.compare(0, cmd.length(), cmd) &&
                (p_s.Mode() & commands[i].mode)) {

            if (CommandMatchCount(cmd) > 1 && dup_chk) {

                if (commands[i].name.length() >= 6) {
                    cerr << "  " << commands[i].name << "\t\t\t" << commands[i].doc << endl;
                } else {
                    cerr << "  " << commands[i].name << "\t\t\t\t" << commands[i].doc << endl;
                }

                current_token = nullptr;

            } else {
                current_token = &commands[i];
            }//if
        }//if
    }//for

    if (current_token && !tokens.empty()) {
        while (!tokens.empty()) {
            cmd = tokens.front();
            tokens.erase(tokens.begin());
            for (int i = 0; i < current_token->sub_command.size(); ++i) {
                if (!current_token->sub_command[i].name.compare(0, cmd.length(), cmd) &&
                        (p_s.Mode() & current_token->sub_command[i].mode) ||
                        current_token->sub_command[i].name[0] == '<' &&
                        (p_s.Mode() & current_token->sub_command[i].mode)) {

                    if (tokens.empty() && SubCommandMatchCount(cmd) == 1) {
                        current_token = &current_token->sub_command[i];
                        return current_token;
                    } else if (!tokens.empty() && SubCommandMatchCount(cmd) == 1) {
                        current_token = &current_token->sub_command[i];
                        break;
                    } else if (SubCommandMatchCount(cmd) > 1 && dup_chk) {
                        if (current_token->sub_command[i].name.length() >= 6) {
                            cerr << "  " << current_token->sub_command[i].name << "\t\t\t"
                                    << current_token->sub_command[i].doc << endl;
                        } else {
                            cerr << "  " << current_token->sub_command[i].name << "\t\t\t\t"
                                    << current_token->sub_command[i].doc << endl;
                        }
                    }//if
                }//if
            }//for
        }//while
    }//if(current_token && !tokens.empty())

    tokens.clear();
    return current_token;
}

/// Count the number of commands for root commands.

int CmdParser::CommandMatchCount(std::string &name) {

    int cnt = 0;
    for (int i = 0; i < commands.size(); i++) {
        if (!commands[i].name.compare(0, name.length(), name) &&
                (p_s.Mode() & commands[i].mode))++cnt;
    }

    return cnt;
}

/// Count the number of matches for sub-commands.

int CmdParser::SubCommandMatchCount(std::string &name) {

    int cnt = 0;
    for (int i = 0; i < current_token->sub_command.size(); i++) {
        if (!current_token->sub_command[i].name.compare(0, name.length(), name) &&
                (p_s.Mode() & current_token->sub_command[i].mode) ||
                current_token->sub_command[i].name[0] == '<' &&
                (p_s.Mode() & current_token->sub_command[i].mode))++cnt;
    }

    return cnt;
}

/*!Attempt to complete command line text.
 *
 * Attempt to complete on the contents of 'text'. 'start' and 'end'
 * bound the region of rl_line_buffer that contains the word to
 * complete. 'text' is the word to complete.  We can use the entire
 * contents of rl_line_buffer in case we want to do some simple
 * parsing. Return the array of matches, or NULL if there aren't any.
 */
char **CmdParser::CommandCompletion(char *text, int start, int end) {

    char **matches = nullptr;

    /* If this word is at the start of the line, then it is a command
       to complete.  Otherwise it is a sub command */

    std::cout << std::endl;

    if (*text) {
        matches = rl_completion_matches(text, CmdParser::CommandGenerate);
    }

    if (matches) {
        if (matches[0][0] == '<')matches = nullptr;
    }

    std::cout << p_s.PS();
    rl_on_new_line_with_prompt();

    return matches;
}

/*!Generator function for command completion.
 *
 * 'state' lets us know whether to start from scratch; without any state
 * (i.e. state == 0), then we start at the top of the list.
 */
char *CmdParser::CommandGenerate(char *text, int state) {

    if (!state) {

        COMMAND *cmd = nullptr;
        istringstream iss(rl_line_buffer);
        iss.clear();

        vector<string> tokens;
        copy(istream_iterator<string > (iss),
                istream_iterator<string > (),
                back_inserter<vector<string> >(tokens));

        cmd = FindCommand(tokens, DupChk);

        if (cmd != nullptr) {
            return dupstr(cmd->name.c_str());
        }
    }

    // If no names matched, then return NULL.
    return nullptr;
}


