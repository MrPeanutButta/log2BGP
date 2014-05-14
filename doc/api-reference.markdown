---
layout: page
title: "API Reference"
date: 2013-11-18 19:32
comments: true
sharing: true
footer: true
---
### Starting the API
The JSON API script can be found in `flowlab_bgp_lg/bgpd/api`. The script name is `bgp_api.rb`. The ruby interpreter will need super user access to be able to bind and listen for requests.

__A very important note about return values:__ ALL values ending in __`_bin`__ are in __Network Byte Order__

* [Endianness](http://en.wikipedia.org/wiki/Endianness)
* [byteorder(3)](https://www.linux.com/learn/docs/man/byteorder3)


>     $cd BigPlg/bigplgd/api
>     $sudo ruby bgp_api.rb
>     == Sinatra/1.4.4 has taken the stage on 4567 for development with backup from Thin
>     >Thin web server (v1.3.1 codename Triple Espresso)
>     >Maximum connections set to 1024
>     >Listening on 0.0.0.0:4567, CTRL+C to stop

or if installed from binary package.

    sudo bigplg-api
    
### API Calls
#### URL and Socket API Calls
[s] = socket call 
[u] = url call
__Bold__ = variable

* [s] api-get prefix __x.x.x.x__
* [u] /bgp/ipv4/__:prefix__
    + get prefix data as it exists now
* [s] api-get prefix __x.x.x.x__ history
* [u] /bgp/ipv4/__:prefix__/history
    + same as above, but fetch historical data also
* [s] api-get all-from-asn __asn__ __length__
* [u] /bgp/ipv4/all/from-asn/__:asn__/__:length__
    + get all prefixes from ASN lessthan or equal to CIDR length; useful for seeing what an AS is advertising.
* [s] api-get all-public-asn __asn__ __length__
* [u] /bgp/ipv4/all/public/from-asn/__:asn__/__:length__
    + get all publicly routable blocks from ASN with lessthan or equal to length
* [s] api-get all-private-asn __asn__ __length__
* [u] /bgp/ipv4/all/private/from-asn/__:asn__/__:length__
    + get all private blocks from ASN with lessthan or equal to length ([rfc1918](https://tools.ietf.org/html/rfc1918#section-3 "rfc1918")); useful if BigPlg has a peering session within the data-center.

### Making API Calls with HTTP
Making API calls is straight forwad. You will need some JSON reader in your application to read the results. You can test your calls by using a JSON read in your web browser.

Just for the sake of simplicity, we will demonstrate the API in a web browser. The URL used is in the format of `http://[serverip]:4567/bgp/ipv4/:prefix`, where __:prefix__ is the IPv4 address you wish to pull data for.<br>
__example:__ entering the following URL in your address bar, where 10.1.1.1 represents the server you are running the bgpd and bgp_api.rb.<br>

    http://10.1.1.1:4567/bgp/ipv4/8.8.8.8

or if needing prefix history

    http://10.1.1.1:4567/bgp/ipv4/8.8.8.8/history

With the above example we might expect a JSON return like so

``` json
    {
        "entries": [
            {
                "prefix": "8.0.0.0",
                "prefix_bin": 8,
                "netmask": "255.0.0.0",
                "netmask_bin": 255,
                "length": 8,
                "prefix_range": "8.255.255.255",
                "prefix_range_bin": 4294967048,
                "peer": "10.12.0.238",
                "peer_name": "route-views1.ord1",
                "current": {
                    "time_str": "2013-11-15 22:54:46",
                    "time_epoch": 1384556086,
                    "as_path_length": 2,
                    "as_path": [
                        "19994(ORD)",
                        "3356(Level3)"
                    ],
                    "origin": "i",
                    "next_hop_str": "(Level3.edge1.ord1)",
                    "next_hop_bin": 905463556,
                    "med": 0,
                    "local_pref": 0,
                    "atomic_aggregate": true,
                    "aggregator_str": "4.69.130.24",
                    "aggregator_bin": 411190532,
                    "aggregator_as_str": "3356(Level3)",
                    "aggregator_as_bin": 3356,
                    "community_length": 7,
                    "community": [
                        "3356(Level3):0",
                        "3356(Level3):3",
                        "3356(Level3):100",
                        "3356(Level3):123",
                        "3356(Level3):575",
                        "3356(Level3):2042",
                        "3356(Level3):11211"
                    ]
                },
            {
                "prefix": "8.8.8.0",
                "prefix_bin": 526344,
                "netmask": "255.255.255.0",
                "netmask_bin": 16777215,
                "length": 24,
                "prefix_range": "8.8.8.255",
                "prefix_range_bin": 4278716424,
                "peer": "10.12.0.238",
                "peer_name": "route-views1.ord1",
                "current": {
                    "time_str": "2013-11-15 22:54:45",
                    "time_epoch": 1384556085,
                    "as_path_length": 3,
                    "as_path": [
                        "19994(ORD)",
                        "3356(Level3)",
                        "15169(Google)"
                    ],
                    "origin": "i",
                    "next_hop_str": "(Level3.edge1.ord1)",
                    "next_hop_bin": 905463556,
                    "med": 0,
                    "local_pref": 0,
                    "community_length": 7,
                    "community": [
                        "3356(Level3):3",
                        "3356(Level3):22",
                        "3356(Level3):86",
                        "3356(Level3):575",
                        "3356(Level3):666",
                        "3356(Level3):2042",
                        "3356(Level3):11211"
                    ]
                }
            }
        ]
    }
```

### Making API Calls with TCP Sockets
Depending on your language of choice this can be done in a number of ways. Ideally you need to know how to open a TCP socket and read and write to it. The most important piece you need to know is the formatting of the `api-get`. Upon completion of an `api-get` the socket is closed by the daemon.

To demonstrate socket API calls I will use telnet. This doesn't mean that it's a telnet daemon, telnet simply connects via TCP and allows us to write directly to the socket.

### Calling Socket API
__Get current prefix data:__ `api-get prefix x.x.x.x`<br>
__Get current and hitorical prefix data:__`api-get prefix x.x.x.x history`

``` bash
    $ telnet localhost 667
    Trying ::1...
    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
     api-get prefix 8.8.8.8
    {"entries":[{"prefix":"8.0.0.0","prefix_bin":8,"netmask":"255.0.0.0","netmask_bin":255,"length":8,"prefix_range":"8.255.255.255","prefix_range_bin":4294967048,"peer":"10.12.0.238","peer_name":"route-views1.ord1","current":{"time_str":"2013-11-15 22:54:46","time_epoch":1384556086,"as_path_length":2,"as_path":["19994(ORD)","3356(Level3)"],"origin":"i","next_hop_str":"(Level3.edge1.ord1)","next_hop_bin":905463556,"med":0,"local_pref":0,"atomic_aggregate":true,"aggregator_str":"4.69.130.24","aggregator_bin":411190532,"aggregator_as_str":"3356(Level3)","aggregator_as_bin":3356,"community_length":7,"community":["3356(Level3):0","3356(Level3):3","3356(Level3):100","3356(Level3):123","3356(Level3):575","3356(Level3):2042","3356(Level3):11211"]}},{"prefix":"8.8.8.0","prefix_bin":526344,"netmask":"255.255.255.0","netmask_bin":16777215,"length":24,"prefix_range":"8.8.8.255","prefix_range_bin":4278716424,"peer":"10.12.0.238","peer_name":"route-views1.ord1","current":{"time_str":"2013-11-15 22:54:45","time_epoch":1384556085,"as_path_length":3,"as_path":["19994(ORD)","3356(Level3)","15169(Google)"],"origin":"i","next_hop_str":"(Level3.edge1.ord1)","next_hop_bin":905463556,"med":0,"local_pref":0,"community_length":7,"community":["3356(Level3):3","3356(Level3):22","3356(Level3):86","3356(Level3):575","3356(Level3):666","3356(Level3):2042","3356(Level3):11211"]}}]}Connection closed by foreign host.
```
#### Socket API example in ruby
``` ruby
    ## BGP Looking Glass Socket API example
    
    require 'socket'      # Sockets are in standard library
    
    def api_write(cmd)
    
      host = '127.0.0.1'
      port = 667
    
      s = TCPSocket.open(host, port)
      
      # Here we need to prepend the string length of the command.
      # The API will expect the length first.
      s.write(cmd.length.chr+cmd.to_s)
    
      ret_val = s.read   # Read lines from the socket
      s.close            # Close the socket when done
      
      return ret_val.to_s
    end
    
    # Current prefix data
    puts("current data for 8.8.8.8 is:")
    puts(api_write("api-get prefix 8.8.8.8"))
    
    # Current prefix data, and historical
    puts("current & historical data for 8.8.8.8 is:")
    puts(api_write("api-get prefix 8.8.8.8 history"))
    
    #FIN
```