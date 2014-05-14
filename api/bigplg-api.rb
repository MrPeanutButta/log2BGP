#!/usr/bin/ruby

## BGP Looking Glass URL API

require 'sinatra'
require 'ipaddr'
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

  #puts(ret_val.to_s)
  return ret_val
end

def is_valid_ipv4(ipv4)
  v4addr = IPAddr.new(ipv4, Socket::AF_INET)
  return v4addr.ipv4?
end

def is_valid_asn(asn)
  return ((asn > 0) && (asn <= 65535))
end

def is_valid_length(length)
  return ((length > 0) && (length <= 32))
end

before '*' do
  content_type 'application/json'
end

# get prefix data as it exists now
get '/bgp/ipv4/:prefix' do
  ip = params[:prefix].to_s

  if is_valid_ipv4(ip)
    write("api-get prefix #{ip}")
  end
end

# same as above, but fetch historical data also
get '/bgp/ipv4/:prefix/history' do
  ip = params[:prefix].to_s

  if is_valid_ipv4(ip)
    write("api-get prefix #{ip} history")
  end
end

# get all blocks public and private with lessthan or eq to length
get '/bgp/ipv4/all/from-asn/:asn/:length' do

  as = params[:asn].to_i
  len = params[:length].to_i

  if is_valid_asn(as) && is_valid_length(len)
    write("api-get all-from-asn #{as} #{len}")
  else
    raise "invalid autonomous system number or prefix length"
  end
end

# get all publicly routable blocks with lessthan or eq to length
get '/bgp/ipv4/all/public/from-asn/:asn/:length' do

  as = params[:asn].to_i
  len = params[:length].to_i

  if is_valid_asn(as) && is_valid_length(len)
    write("api-get all-public-asn #{as} #{len}")
  else
    raise "invalid autonomous system number or prefix length"
  end
end

# get all private blocks with lessthan or eq to length (rfc1918)
get '/bgp/ipv4/all/private/from-asn/:asn/:length' do

  as = params[:asn].to_i
  len = params[:length].to_i

  if is_valid_asn(as) && is_valid_length(len)
    write("api-get all-private-asn #{as} #{len}")
  else
    raise "invalid autonomous system number or prefix length"
  end
end

# get all blocks marked for no-export/no-advertise from AS
# with lessthan or eq to length
#get '/bgp/ipv4/all/no-export/no-advertise/from-asn/:asn/:length' do
#  write("api-get private-from-asn #{params[:asn]} #{params[:length]}")
#end
#
#get '/bgp/ipv4/:prefix/:time_start/:time_end' do
#  write("api-get prefix-events #{params[:prefix]} #{params[:time_Start]} #{params[:time_end]}")
#end
