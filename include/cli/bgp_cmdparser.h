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

#include <string>
#include <vector>

class BGP;

/// Utility command line functions
namespace proto_bgp {
    /// Pointer to BGP::Member function
    typedef std::string(*cmd_BGP_func_v)(BGP *, std::vector<std::string> &);

};

/*!A structure which contains information on the commands this class
 * can understand.
 */
struct COMMAND {
    /// User printable name of the function.
    std::string name;
    /// Function to call to do the job.
    proto_bgp::cmd_BGP_func_v func;
    /// Sub commands.
    std::vector<COMMAND> sub_command;
};

/// Command line parsing class.
namespace command_parser {

    class cmd_parser {
    public:

        friend class BGP;

        explicit cmd_parser();
        virtual ~cmd_parser() = 0;

        std::string ExecuteLine(BGP *, const char *);
        static COMMAND const *FindCommand(std::vector<std::string>);
        static void CmdInit(void);

        /// The command tree.
        static std::vector<COMMAND> commands;
        /// Pointer to the current COMMAND.
        static COMMAND const *current_token;
    };

}

#endif
