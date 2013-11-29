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

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include "defines.h"

/*!Prompt mode bit flags.
 *
 * 0x01\t==\t"00000001"\n
 * 0x02\t==\t"00000010"\n
 * 0x04\t==\t"00000100"\n
 * 0x08\t==\t"00001000"\n
 * 0x10\t==\t"00010000"\n
 * 0x20\t==\t"00100000"\n
 * 0x40\t==\t"01000000"\n
 * 0x80\t==\t"10000000"\n
 */
enum prompt_mode {
    USEREXEC = 0x01,
    PRIVEXEC = 0x02,
    CONFIG = 0x04,
    CONFIGINT = 0x08,
    CONFIGLINE = 0x10,
    CONFIGPROTO = 0x20,
    CONFIGPROTOBGP = 0x40,
    SUBCOMMAND = USEREXEC | PRIVEXEC | CONFIG | CONFIGINT | CONFIGLINE | CONFIGPROTO | CONFIGPROTOBGP
};

/*!Command prompt structure.
 *
 * 'prompt' Mode() controls which commands are available to the user.
 * 'prompt' PS() changes to alert user to which Mode() they are currently in.
 */
class prompt {
public:
    /// Enter privileged exec mode.

    void ExecPrompt(void) {
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        config_ps2 = false;
        priv_exec_ps2 = true;
        mode_p = PRIVEXEC;
    };

    /// Enter user exec mode.

    void UserPrompt(void) {
        priv_exec_ps2 = false;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        config_ps2 = false;
        mode_p = USEREXEC;
    };

    /// Enter configuration exec mode.

    void EnterConfig(void) {
        priv_exec_ps2 = false;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        config_ps2 = true;
        mode_p = CONFIG;
    };

    /// Exit configuration exec mode.

    void ExitConfig(void) {
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        config_ps2 = false;
        priv_exec_ps2 = true;
        mode_p = PRIVEXEC;
    };

    /// Enter interface configuration exec mode.

    void EnterConfigInt(void) {
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        config_ps2 = false;
        priv_exec_ps2 = false;
        config_int_ps2 = true;
        mode_p = CONFIGINT;
    };

    /// Exit interface configuration exec mode.

    void ExitConfigInt(void) {
        config_ps2 = true;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIG;
    };

    /// Enter line 'vty/con' configuration exec mode.

    void EnterConfigLine(void) {
        config_ps2 = false;
        config_line_ps2 = true;
        config_int_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIGLINE;
    };

    /// Exit line 'vty/con' configuration exec mode.

    void ExitConfigLine(void) {
        config_ps2 = true;
        config_line_ps2 = false;
        config_int_ps2 = false;
        config_proto_ps2 = false;
        config_proto_bgp_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIG;
    };

    /// Enter proto configuration exec mode.

    void EnterConfigProto(void) {
        config_ps2 = false;
        config_proto_ps2 = true;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_bgp_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIGPROTO;
    };

    /// Exit proto configuration exec mode.

    void ExitConfigProto(void) {
        config_ps2 = true;
        config_proto_ps2 = false;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_bgp_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIG;
    };

    /// Enter proto bgp configuration exec mode.

    void EnterConfigProtoBGP(void) {
        config_ps2 = false;
        config_proto_bgp_ps2 = true;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIGPROTOBGP;
    };

    /// Exit proto bgp configuration exec mode.

    void ExitConfigProtoBGP(void) {
        config_ps2 = true;
        config_proto_bgp_ps2 = false;
        config_int_ps2 = false;
        config_line_ps2 = false;
        config_proto_ps2 = false;
        priv_exec_ps2 = false;
        mode_p = CONFIG;
    };

    /*!Exit one level of mode.
     *
     * If at base level, e.g USEREXEC/PRIVEXEC, clean up and exit
     */
    void ExitOneLevel(void) {

        switch (mode_p) {
            case CONFIG:
                ExitConfig();
                break;
            case CONFIGINT:
                ExitConfigInt();
                break;
            case CONFIGLINE:
                ExitConfigLine();
                break;
            case CONFIGPROTO:
                ExitConfigProto();
                break;
            case CONFIGPROTOBGP:
                ExitConfigProtoBGP();
                break;
            default:
                exit(0);
                break;
        };
    };

    /// Return char * value of the current prompt.

    char *PS(void) const {

        if (priv_exec_ps2)
            return ps2;
        else if (config_ps2)
            return config_p;
        else if (config_int_ps2)
            return config_int_p;
        else if (config_line_ps2)
            return config_line_p;
        else if (config_proto_ps2)
            return config_proto_p;
        else if (config_proto_bgp_ps2)
            return config_proto_bgp_p;
        else
            return ps1;

    };

    /// Return the current prompt_mode.

    prompt_mode Mode(void) const {
        return mode_p;
    };

    /// Init the default values for exec prompts.

    void PSInit(void) {

        memset(&hostname, 0, sizeof (hostname));

        gethostname(hostname, 50);

        hostname[strlen(hostname)] = '\0';

        strcpy(ps1, hostname);
        strcpy(ps2, hostname);
        strcpy(config_p, hostname);
        strcpy(config_int_p, hostname);
        strcpy(config_line_p, hostname);
        strcpy(config_proto_p, hostname);
        strcpy(config_proto_bgp_p, hostname);


        ps1[strlen(ps1)] = '>';
        ps2[strlen(ps2)] = '#';

        strcat(config_p, "(config)#");
        strcat(config_int_p, "(config-int)#");
        strcat(config_line_p, "(config-line)#");
        strcat(config_proto_p, "(config-proto)#");
        strcat(config_proto_bgp_p, "(config-proto-bgp)#");

    };

private:
    bool priv_exec_ps2;
    bool config_ps2;
    bool config_int_ps2;
    bool config_line_ps2;
    bool config_proto_ps2;
    bool config_proto_bgp_ps2;

    char cmd_buffer[1024];
    char ps1[50];
    char ps2[50];
    char hostname[50];
    char config_p[50];
    char config_int_p[50];
    char config_line_p[50];
    char config_proto_p[50];
    char config_proto_bgp_p[50];

    prompt_mode mode_p;
};

/// Utility command line functions
namespace Utility {
    /// Pointer to Util::Member function
    typedef int (*cmd_util_func_v)(std::vector<std::string> &);
    // pointer to daemon reader
    typedef void (*daemon_reader)(const char *, const char *);
    // pointer to member that requires grp
    typedef void (*cmd_grep_func_v)(cmd_util_func_v, std::vector<std::string> &, daemon_reader);

};

/*!A structure which contains information on the commands this class
 * can understand.
 */
struct COMMAND {
    /// User printable name of the function.
    std::string name;
    /// Function to call to do the job.
    Utility::cmd_util_func_v func;
    /// Documentation for this function.
    std::string doc;
    /// Prompt mode level of exec.
    prompt_mode mode;
    /// Sub commands.
    std::vector<COMMAND> sub_command;
};

namespace CommandParser {
    /// Command line parsing class.

    class CmdParser {
    public:
        static int ExecuteLine(char *);

        static char **CommandCompletion(char *, int, int);
        static char *CommandCompletionEntry(const char *, int);
        static char *CommandGenerate(char *, int);
        static int CommandMatchCount(std::string &);
        static int SubCommandMatchCount(std::string &);

        static COMMAND *FindCommand(std::vector<std::string>, const bool dup_chk = true);

        static char **CommandMatches(std::string &, int &, int &);

        static void CmdInit(void);

        CmdParser();

        virtual ~CmdParser() = 0;

        static void InitReadLine(void);

        /// The command tree.
        static std::vector<COMMAND> commands;

        /// Pointer to the current COMMAND.
        static COMMAND const *current_token;

        static const bool DupChk = true;
        static const bool NoDupChk = false;
        static int psuedo_term;
        static std::string psuedo_term_in;
        static std::string psuedo_term_out;
        static prompt p_s;

        inline static std::string trim_right_copy(
                const std::string& s,
                const std::string& delimiters = " \f\n\r\t\v") {

            if (s.empty()) return "";

            return s.substr(0, s.find_last_not_of(delimiters) + 1);
        }

        inline static std::string trim_left_copy(
                const std::string& s,
                const std::string& delimiters = " \f\n\r\t\v") {

            if (s.empty()) return "";

            return s.substr(s.find_first_not_of(delimiters));
        }

        inline static std::string trim_copy(
                const std::string& s,
                const std::string& delimiters = " \f\n\r\t\v") {

            if (s.empty()) return "";

            return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
        }

    };
}
#endif
