#!/usr/bin/ruby

## BGP Looking Glass URL API

require 'sinatra'
require 'json'
require 'socket'      # Sockets are in standard library

set :port, 4567
set :bind, '0.0.0.0'
set :protection, :except => :json_csrf

def write(cmd)

  host = '127.0.0.1'
  port = 667

  s = TCPSocket.open(host, port)
  s.write(cmd.length.chr+cmd.to_s)

  ret_val = s.read   # Read lines from the socket
  s.close            # Close the socket when done

  puts(ret_val.to_s)
  return ret_val
end

before '*' do
  content_type 'application/json'
end

# get prefix data as it exists now
get '/bgp/ipv4/:prefix' do
  write("api-get prefix #{params[:prefix]}")
end

# same as above, but fetch historical data also
get '/bgp/ipv4/:prefix/history' do
  write("api-get prefix #{params[:prefix]} history")
end

# get all blocks public and private with lessthan or eq to length
get '/bgp/ipv4/all/from-asn/:asn/:length' do
  write("api-get all-from-asn #{params[:asn]} #{params[:length]}")
end

# get all publicly routable blocks with lessthan or eq to length
get '/bgp/ipv4/all/public/from-asn/:asn/:length' do
  write("api-get public-from-asn #{params[:asn]} #{params[:length]}")
end

# get all private blocks with lessthan or eq to length (rfc1918)
get '/bgp/ipv4/all/private/from-asn/:asn/:length' do
  write("api-get private-from-asn #{params[:asn]} #{params[:length]}")
end

# get all blocks marked for no-export/no-advertise from AS
# with lessthan or eq to length
get '/bgp/ipv4/all/no-export/no-advertise/from-asn/:asn/:length' do
  write("api-get private-from-asn #{params[:asn]} #{params[:length]}")
end

get '/bgp/ipv4/:prefix/:time_start/:time_end' do
  write("api-get prefix-events #{params[:prefix]} #{params[:time_Start]} #{params[:time_end]}")
end
