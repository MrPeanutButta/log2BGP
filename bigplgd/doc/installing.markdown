---
layout: page
title: "Installing"
date: 2013-11-10 05:48
comments: true
sharing: true
footer: true
---
## Building and Installing Flowlab from GIT repo
### Prerequisites:
* Debian 3.2.51-1 x86_64
* g++ (Debian 4.8.2-1) 4.8.2

#### bgpd
* libpthread.so.0
* libstdc++.so.6
* libm.so.6
* libgcc_s.so.1
* libc.so.6
* ruby 1.9.3p448 (2013-06-27 revision 41675) [x86_64-linux]
* sinatra (1.4.4, 1.4.2)

#### cli (will be depricated)
* libreadline.so.6
* libutil.so.1
* libpthread.so.0
* libstdc++.so.6
* libm.so.6
* libgcc_s.so.1
* libc.so.6

## Installing Prereqs
__ruby:__ 			``sudo apt-get install ruby``<br>
__sinatra:__ 		``gem install sinatra``<br>
__libreadline:__ 	``sudo apt-get install libreadline-dev``

## Installing Binary Releases


## Compiling the Source
Download from GitHub: http://ahebert.github.io/flowlab_bgp_lg/

        git clone git://github.com/ahebert/flowlab_bgp_lg.git
        cd flowlab_bgp_lg
        make

bgpd and flowlab-cli will be built in the `dist` directories. There may be a need to compile flowlab-cli with `-D LOOKING_GLASS_ONLY_CLI` to allow read-only access to certain users. It's suggested that this read-only CLI be named flowlab-lg-cli to discern between the admin console and user console.

### Compiling Options
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


## Creating a User Account
Create a user account which points to /bin/flowlab-cli as its shell. This will allow the OS to provide authentication for us whether it's PAM or TACACS+.

        sudo useradd flowlab-user -M -N -s /bin/flowlab-cli -p somepassword

## Testing the Shell Account
Test the new user account by SSH or Telnet. Whatever service you have configured, flowlab-cli should be the shell for the new user.

        ssh -l flowlab-user 10.1.1.1

## Configuring syslog (optional)
This syslogd config can be copied to `/etc/rsyslog.conf`

        #flowlab - custom syslogging
        local7.emerg            /var/log/flowlab-0.log
        local7.alert            /var/log/flowlab-1.log
        local7.crit             /var/log/flowlab-2.log
        local7.err              /var/log/flowlab-3.log
        local7.warn             /var/log/flowlab-4.log
        local7.notice           /var/log/flowlab-5.log
        local7.info             /var/log/flowlab-6.log
        local7.debug            /var/log/flowlab-7.log