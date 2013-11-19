#!/usr/bin/ruby

## BGP Looking Glass URL API

require 'sinatra'
require 'json'
require 'socket'      # Sockets are in standard library

set :port, 4567
set :bind, '0.0.0.0'

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

get '/bgp/ipv4/:prefix' do
  write("api-get prefix #{params[:prefix]}")
end

get '/bgp/ipv4/:prefix/history' do
  write("api-get prefix #{params[:prefix]} history")
end
