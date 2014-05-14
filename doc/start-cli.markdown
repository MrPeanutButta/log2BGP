### Command-line Interface
The command line interface to log2CLI loosely follows a familiar Cisco IOS style syntax. There are also various modes of operation that will allow user or admin level commands to be entered. The CLI can be started with `sudo`.

    sudo log2cli

### User Exec
The user exec prompt is initially set to 0x01 which allows common commands to be accessed. Below is an example of just a few commands that would be visible to the non admin user. 

        ahebert-M15x>
          ?                             synonym for quick/command `help'
          cd                            change directory
          enable                        enter privileged exec mode
          exit                          exit current configuration level
          help                          display this text
          list                          list files in directory
          ls                            synonym for `list'
          ping                          ping destination address or hostname
          pwd                           print working directory
          show                          show running system information
          stat                          print out statistics on a file

The quick help '?' will display a list of available commands. If the text already existing on the command line is unique, then the list of sub commands is returned.

        ahebert-M15x>enable 
          <cr>

### Privileged Exec
The user may enter admin mode by entering the 'enable' command. The user is then prompted to enter the local system password for that account. This is the equivalent of entering 'sudo' from the bash shell.

        ahebert-M15x>enable 
        Password: 
        ahebert-M15x#

After entering admin mode, more commands become available to perform system tasks such as entering configuration mode.

        ahebert-M15x#
          ?                             synonym for quick/command `help'
          cd                            change directory
          configure                     enter configuration mode
          delete                        delete a file
          disable                       exit privileged exec mode
          exit                          exit current configuration level
          help                          display this text
          list                          list files in directory
          ls                            synonym for `list'
          ping                          ping destination address or hostname
          pwd                           print working directory
          rename                        rename a file
          show                          show running system information
          stat                          srint out statistics on a file
          write                         write running-config

### Configuration Mode
System configuration is performed from the configuration prompt. From this mode interfaces, protocols, and local system settings can be configured.

        ahebert-M15x#configure terminal 
          <cr>

### Command line editing
Various hot-keys allow command line editing. 
A list of a few helpful hot-keys.

* TAB : command completion
* ctrl + a : go to beginning of command line
* ctrl + e : go to end of command line
* up/down arrow : scroll through command history
* ? : display available completions or commands