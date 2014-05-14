### A note about example configurations
The below configurations are just examples and do not reflect a full configuration. Hopefully these examples will convey the basic idea, but they are not meant for a working configuration.

### Loading initial configuration
An initial configuration can be loaded with the `-load` switch.<br>

    log2cli -load config-file-name

### Configuring peering sessions
The CLI syntax for peering closely resembles the Cisco IOS syntax. Below is an example. Note, that the BGP ID must be statically set before the session will establish.

        ! comments are beginning with "bang" !
        ! enter configuration mode
        configure terminal
        !
        ! turn on BGP protocol AS #65301
        protocol bgp 65301
        !
        ! statically set BGP ID
        bgp-id 10.1.2.16
        !
        ! declare a peer in remote AS 65000
        neighbor 10.1.2.15 remote-as 65000

### Setting TCP and BGP buffers
Increasing the buffer sizes may yield better performance if the BGP peer is expected to send lots of updates. However, increasing these buffers will add to the memory overhead and if the session is already established, may take some time depending on the amount of space requested for allocation.

        ! assuming 10.1.2.15 is already a peer
        ! peer socket and stream buffer <512 min - 16384 max>
        neighbor 10.1.2.15 rx-buffer 16384 16384
        ! smallest send buffers to conserve resources. flowlab does not send UPDATE messages (yet)
        neighbor 10.1.2.15 tx-buffer 512 512

### Aliasing BGP attributes
To make the BGP update log a bit more readable, through the CLI one can configure attributes with a alias.
Assuming our peering session is already configured:
#### Alias BGP ID of the peer

        neighbor 10.1.2.15 attribute bgp-id alias route-reflector(lab)

#### Alias AS_PATH attribute
If the AS value appears in the AS_PATH, a string label will be appended.

        neighbor 10.1.2.15 attribute as-path alias 65001 (lab1)
        neighbor 10.1.2.15 attribute as-path alias 65002 (lab2)
        neighbor 10.1.2.15 attribute as-path alias 65003 (lab3)
        neighbor 10.1.2.15 attribute as-path alias 65004 (lab4)
        neighbor 10.1.2.15 attribute as-path alias 65005 (lab5)
        neighbor 10.1.2.15 attribute as-path alias 65006 (lab6)
        neighbor 10.1.2.15 attribute as-path alias 65007 (lab7)
        neighbor 10.1.2.15 attribute as-path alias 65008 (lab8)
        neighbor 10.1.2.15 attribute as-path alias 3356 (Level3)
        neighbor 10.1.2.15 attribute as-path alias 209 (CenturyLink)
        neighbor 10.1.2.15 attribute as-path alias 4436 (nLayer)
        neighbor 10.1.2.15 attribute as-path alias 6461 (Zayo)
        neighbor 10.1.2.15 attribute as-path alias 1299 (Telia)
        neighbor 10.1.2.15 attribute as-path alias 174 (Cogent)

#### Alias NEXT_HOP attribute
When logging updates, it may be helpful to see actual device names instead of IP address.

        neighbor 10.1.2.15 attribute next-hop alias ipv4 4.7.24.5 (Level3.edge1)
        neighbor 10.1.2.15 attribute next-hop alias ipv4 65.11.20.12 (CenturyLink.edge2) 
        neighbor 10.1.2.15 attribute next-hop alias ipv4 69.3.11.21 (nLayer.edge2)
        neighbor 10.1.2.15 attribute next-hop alias ipv4 69.1.11.54 (nLayer_nullroute.edge2)   
        neighbor 10.1.2.15 attribute next-hop alias ipv4 64.14.6.21 (Zayo.edge3)
        neighbor 10.1.2.15 attribute next-hop alias ipv4 80.39.10.9 (Telia.edge3)
        neighbor 10.1.2.15 attribute next-hop alias ipv4 38.22.10.5 (Cogent.edge4)
        neighbor 10.1.2.15 attribute next-hop alias ipv4 66.8.8.1 (Cogent_nullroute.edge4)

#### Alias COMMUNITY attribute
If wanting to track prefixes that may have been set with a community string, aliasing community values is supported. Note that if the AS_PATH attribute is already configured, then only the VALUE needs to be set.
Below is an example of how to track prefixes that are being manipulated with a BGP "flow-control" device.
Any prefix that gets manipulated gets a community of $AS:8000. We match the value 8000.

        neighbor 10.1.2.15 attribute community alias 8000 (FCP)

### Viewing peer information
BGP peer information can be viewed with `show bgp neighbors `

        RAX-looking-glass>show bgp neighbors 

        BGP neighbor is 10.1.2.15, remote AS 65000, external link
          BGP version 4, remote router ID rr1b.lab1
          BGP state = Established, up for 17:10:57
          Last read 00:00:09, last write 00:00:09, hold time is 180, keepalive interval is 60 seconds
          Minimum holdtime from neighbor is 0 seconds
          Message statistics:
            InQ depth is 0
            OutQ depth is 0
                                Sent            Rcvd
            Opens:                     1               1
            Notifications:             0               0
            Updates:                   0           219749
            Keepalives:             2487               2
            Route Refresh:             0               0
            Total:                  2488           219752

          TCP RCV socket size (bytes)

                                System          Config
                                   29200           14600

          TCP SND socket size (bytes)

                                System          Config
                                   29200           14600

        BGP neighbor is 10.6.1.17, remote AS 65001, external link
          BGP version 4, remote router ID rr1b.lab2
          BGP state = Established, up for 17:09:19
          Last read 00:00:10, last write 00:00:10, hold time is 180, keepalive interval is 60 seconds
          Minimum holdtime from neighbor is 0 seconds
          Message statistics:
            InQ depth is 0
            OutQ depth is 0
                                Sent            Rcvd
            Opens:                     1               1
            Notifications:             0               0
            Updates:                   0           185992
            Keepalives:             2304               2
            Route Refresh:             0               0
            Total:                  2305           185995

          TCP RCV socket size (bytes)

                                System          Config
                                   29200           14600

          TCP SND socket size (bytes)

                                System          Config
                                   29200           14600

### Prefix lookup (history)
`show bgp ipv4 <x.x.x.x>` - Will return all matches.<br>
`show bgp ipv4 <x.x.x.x> history` - Will return all matches with historical data.

        RAX-looking-glass>show bgp ip 1.4.128.0 history

        Entries: (4 available)

        BGP routing table entry for 1.4.128.0/17 : (1.4.128.0 - 1.4.255.255)
        Received from 10.4.2.111(route-views1.iad1)
                Last Modified: Mon Jun  3 22:19:42 2013
                Path: 27357(IAD) 3356(Level3) 38040 9737 i
                Next hop: (Level3.edge3.iad1) Metric: 0 Local Pref: 0  Atomic Aggregate: true
                Aggregator Origin: 203.113.12.253 Aggregator AS: 9737

        [h]     Last Modified: Mon Jun  3 22:19:41 2013
                Path: 27357(IAD) 3356(Level3) 7018 38040 9737 i
                Next hop: (Level3.edge5.iad3) Metric: 0 Local Pref: 0 

        BGP routing table entry for 1.4.128.0/17 : (1.4.128.0 - 1.4.255.255)
        Received from 10.7.12.248(route-views1.dfw1)
                Last Modified: Mon Jun  3 22:19:36 2013
                Path: 33070(DFW) 1299(Telia) 38040 9737 i
                Next hop: (Telia.edge5.dfw1) Metric: 0 Local Pref: 0  Atomic Aggregate: true
                Aggregator Origin: 203.113.12.253 Aggregator AS: 9737

        [h]     Last Modified: Mon Jun  3 22:19:34 2013
                Path: 33070(DFW) 1299(Telia) 7018 38040 9737 i
                Next hop: (Telia.edge5.dfw1) Metric: 0 Local Pref: 0 

        BGP routing table entry for 1.4.128.0/19 : (1.4.128.0 - 1.4.159.255)
        Received from 10.9.2.35(route-views1.lon3)
                Last Modified: Mon Jun  3 22:20:23 2013
                Path: 15395(LON) 6453(Tata) 7018 38040 9737 i
                Next hop: (Tata.edge3.lon) Metric: 0 Local Pref: 0 
                Community: 15395(LON):6453 

        [h]     Last Modified: Mon Jun  3 22:20:14 2013
                Path: 15395(LON) 3356(Level3) 38040 9737 i
                Next hop: (Level3.edge1.lon) Metric: 0 Local Pref: 0  Atomic Aggregate: true
                Aggregator Origin: 203.113.12.253 Aggregator AS: 9737
                Community: 15395(LON):3356 

        BGP routing table entry for 1.4.128.0/19 : (1.4.128.0 - 1.4.159.255)
        Received from 10.12.0.238(route-views1.ord1)
                Last Modified: Mon Jun  3 22:19:35 2013
                Path: 19994(ORD) 174(Cogent) 7018 38040 9737 i
                Next hop: (Cogent.edge4.ord1) Metric: 0 Local Pref: 0 
                Community: 174(Cogent):21000 174(Cogent):22013 

        [h]     Last Modified: Mon Jun  3 22:19:35 2013
                Path: 19994(ORD) 3356(Level3) 38040 9737 i
                Next hop: (Level3.edge1.ord1) Metric: 0 Local Pref: 0  Atomic Aggregate: true
                Aggregator Origin: 203.113.12.253 Aggregator AS: 9737

### Viewing the full adjacency RIB table
The full adj-rib-in may be viewed with `show bgp adj-rib-in`

        RAX-looking-glass#show bgp adj-rib-in 

        BGP table version is 2008008, local router ID is 10.12.125.231
        Status codes: s suppressed, d damped, h history, * valid, > best, i - internal, 
                r RIB-failure, S Stale
        Origin codes: i - IGP, e - EGP, ? - incomplete

        RIB total size          = 103008944(bytes allocated) 
        RIB max size            = 109802048057794950(entries)
        RIB updates in queue            =0
        RIB withdraws in queue          =0

           Network        Next Hop           Metric LocPrf Weight  Path
           1.0.0.0/24     195.50.122.41           0      0      0  15395 3356 15169 i
           1.0.0.0/24     69.31.110.241           0      0      0  19994 4436 15169 i
           1.0.4.0/24     195.219.83.177          0      0      0  15395 6453 4323 7545 7545 7545 7545 56203 i
           1.0.4.0/22     10.12.0.5               0      0      0  19994 36248 6939 9482 38803 56203 56203 56203 i
           1.0.5.0/24     195.219.83.177          0      0      0  15395 6453 4323 7545 7545 7545 7545 56203 i
           1.0.5.0/24     10.12.0.5               0      0      0  19994 36248 6939 7545 7545 7545 7545 7545 56203 i
           1.0.6.0/24     195.219.83.177          0      0      0  15395 6453 4323 7545 7545 7545 7545 56203 i
           1.0.6.0/24     10.12.0.5               0      0      0  19994 36248 6939 7545 7545 7545 7545 7545 56203 i
           1.0.7.0/24     195.219.83.177          0      0      0  15395 6453 4323 7545 7545 7545 7545 56203 i
           1.0.7.0/24     10.12.0.5               0      0      0  19994 36248 6939 7545 7545 7545 7545 7545 56203 i
           1.0.20.0/23    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.0.20.0/23    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.0.22.0/23    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.0.22.0/23    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.0.24.0/23    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.0.24.0/23    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.0.26.0/23    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.0.26.0/23    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.0.28.0/22    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.0.28.0/22    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.0.64.0/18    195.219.83.177          0      0      0  15395 6453 2497 7670 18144 i
           1.0.64.0/18    4.71.248.53             0      0      0  19994 3356 2516 7670 18144 i
           1.0.128.0/18   195.50.122.41           0      0      0  15395 3356 38040 9737 i
           1.0.128.0/18   69.31.110.241           0      0      0  19994 4436 701 38040 9737 i
           1.0.224.0/19   195.219.83.177          0      0      0  15395 6453 1239 38040 9737 i
           1.0.224.0/19   38.122.180.5            0      0      0  19994 174 1239 38040 9737 i
           1.1.1.0/24     195.50.122.41           0      0      0  15395 3356 15169 i
           1.1.1.0/24     69.31.110.241           0      0      0  19994 4436 15169 i
           1.1.64.0/19    195.50.122.41           0      0      0  15395 3356 2516 2519 i
           1.1.64.0/19    80.239.160.49           0      0      0  19994 1299 2914 2519 i
           1.1.127.0/24   195.50.122.41           0      0      0  15395 3356 2516 2519 i
        ---output omited---