### Building and Installing from GIT repo
### Prerequisites:
* Debian 3.2.51-1 x86_64
* g++ (Debian 4.8.2-1) 4.8.2

#### log2BGP
* libpthread.so.0
* libstdc++.so.6
* libm.so.6
* libgcc_s.so.1
* libc.so.6
* ruby 1.9.3p448 (2013-06-27 revision 41675) [x86_64-linux]
* sinatra (1.4.4, 1.4.2)

### Installing Binary Releases
You may install binary releases with `dpkg`.

    dpkg -i log2bgp*.deb

#### Releases
* NONE

### Installing Prereqs and Compiling the Source (Ubuntu)
The `build_bigplg` script will attempt to install all prerequisites and call make to compile the project.

    $ git clone https://github.com/log2co/log2BGP.git
    $ cd log2BGP
    $ ./build_bigplg

#### Compiling Options
The follow can be specified by using the -D compiler flag.

* `BGP_GDB_DEBUG_NO_DAEMON`         : If defined, runs bgpd in the foreground.
                                    Useful if step debugging.
* `BGP_GDB_DEBUG_TEST_PACKET`       : Include specially crafted packets for testing.
* `BGP_GDB_DEBUG_RIB_PROFILE`       : Profile RIB updates in seconds
* `TCP_CLIENT_BGPD`                 : Creates pure virtual dtor for tcp_client. Needed
                                    for bgpd since BGP inherits tcp_client.
                                    virtual ~tcp_client() = 0;
* `WITHDRAW_LOGGING`                : Turns on explicit withdraw logging.
* `BGP_UNIT_TEST`                   : Generic unit testing.
* `USE_ESTABLISHED_TO_PUSH_QUEUE`   : Defined to use BGP::peer_established(BGP *bgp, bgp_peer *peer)
                                    to push test packets to peer queue.
* `USE_BGP`							: Enables BGP misc.cpp helper functions.


### Creating a User Account
Create a user account which points to /bin/log2cli as its shell. This will allow the OS to provide authentication for us whether it's PAM or TACACS+.

        sudo useradd log2bgp-user -M -N -s /usr/sbin/log2cli -p somepassword

### Testing the Shell Account
Test the new user account by SSH or Telnet. Whatever service you have configured, `log2cli` should be the shell for the new user.

        ssh -l log2bgp-user 10.1.1.1

### Configuring syslog (optional)
This syslogd config can be copied to `/etc/rsyslog.conf`

        #flowlab - custom syslogging
        local7.emerg            /var/log/log2bgp-0.log
        local7.alert            /var/log/log2bgp-1.log
        local7.crit             /var/log/log2bgp-2.log
        local7.err              /var/log/log2bgp-3.log
        local7.warn             /var/log/log2bgp-4.log
        local7.notice           /var/log/log2bgp-5.log
        local7.info             /var/log/log2bgp-6.log
        local7.debug            /var/log/log2bgp-7.log