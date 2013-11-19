## Border Gateway Protocol: A transparent looking-glass
This BGP looking-glass allows the collection of the IPv4 Internet 
routing table which may be viewed by service providers and/or ISP customers to validate 
IP routing across autonomous systems.

Routing updates may also have attributes aliased to become (graph) readable. This function 
exists to allow the exporting of RIB updates to splunk, or any such data collector.

A beneficial side-effect of collecting BGP routing information is the ability to track BGP updates/events local or remote(upstream). To analyze events splunk is supported. A JSON/Socket API is currently under development.

Custom graphs can be easily implemented if taken the time to do so. There's
certainly no need to rely on splunk if one developed another way of displaying
data. This project is meant to be nothing more than a route collector, and 
it's interface is solely CLI(command line interface). This project does not have
graph data "out of the box".

![bgplg](http://aaronhebert.net/bgplg/images/screeny.png?raw=true)

### Enhancing Automation
On large-scale networks, this daemon may serve as a BGP routing information base for scripts to harvest network information. This eliminates the need to "screen scrape" output with regex, which always leaves me feeling dirty. A JSON/Socket API is currently under development to serve this purpose.

### Documentation
* **[Installing](http://aaronhebert.net/bgplg/installing.html)**
* **[Command-line Basics](http://aaronhebert.net/bgplg/start-cli.html)** 
* **[Command-line Refence](http://aaronhebert.net/bgplg/cli-reference.html)**
* **[REST API Reference](http://aaronhebert.net/bgplg/api-reference.html)**

## Release Notes on bgpd v1.0.3(captain's log)
New capabilities in this revision, and 1 bug fix.

__NOTE:__ I've opted to use the daemons current binary database as a means of
indexing BGP updates. These entries can be queried via a URL served by the ruby
library Sinatra, or directly served via TCP socket API call.

BGP databases are organized by index. Each prefix may occupy up to 1000 entries. Once the max entry limit is reached (1000 entries), the oldest entries will begin to be overwritten. It's recommended that you export to some log collector if you wish to keep the data.

### Revision 1.0.3 Notes

### Additions
* Created version definitions in `version.h`.
* Created TCP listener for Sinatra/socket API
* Converts all API call returns to JSON formatted strings.
* Supports wide(2byte) and narrow(1byte) character TCP sockets. e.g Unicode support
* Reaching the sinatra API:
	+ Current:
		+ http://[serverip]:4567/bgp/ipv4/:prefix
	+ History:
		+ http://[serverip]:4567/bgp/ipv4/:prefix/history
* Reaching the TCP socket level API:
	+ open TCP socket to [serverip] port 667
    	+ write command length followed by api-get
    	+ can be tested with `telnet localhost 667`

```
    // Raw API sockets
    // pseudo code
    
    server = 10.1.1.1;
    port = 667;
    
    api_cmd = "api-get prefix 8.8.8.8";
    length = string_length(api_cmd);
    
    tcp_stream = open_socket(server, port);
    
    write(tcp_stream, length);
    write(tcp_stream, api_cmd);
    
    str_return = read(tcp_stream);
```
### Revisions
* Peer state machine revised to only use 1 dedicated thread for reads, and the main BGP thread handles all queue processing. This has reduced the number of threads from 2 per peer to 1.
* Collapsed TCP server loops for CLI and API into a single method.
This will reduce code bloat.

### Bug fixes
* bgpd failed to create it's withdraw database if the file didn't already exist. Failure to open it's database resulted in BGP event data not being collected.

### TODO (in progress)

* Add more functionality to the API from this point forward.
* Doxygen code comment clean up.
* Hosting Doxygen on projects webpage.
* Make max database entries configurable instead of hard-coded limit 1000.
* Finish [cx-lang](https://github.com/ahebert/Cx) project enough to use in configuring bigplgd.

### Road-map (up coming plans)
* IPv6 and multi-protocol extensions.
* BGP Additional Paths support. [Cisco Documentation](http://www.cisco.com/en/US/docs/ios-xml/ios/iproute_bgp/configuration/xe-3s/asr1000/irg-additional-paths.html)
* The CLI application responsible for configuring `bgpd` __will be deprecated at some point in the future__. CLI will be replaced with a configuration language that will allow start-up and run-time configurations via configuration scripts. Yep, CLI is going away.
* Web front end for pulling JSON from the API and displaying in a friendly UI.
* Considering graphing BGP event data for the web front end.

### Bug Reporting
See [issues](https://github.com/ahebert/flowlab_bgp_lg/issues) to report issues and check for updates on what is currently being worked.

### Contributing
If you wish to contribute, please fork the repo and contact <aaron.hebert@gmail.com> if you have any questions.