#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import socket

BUF_SIZE = 1024

ip = '127.0.0.1'
port = 8888

#server_addr = ('127.0.0.1', 8888)
server_addr = (ip, port)

print "Helloi, tcp/ip network start!"

try:
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg :
	print "Create socket failed. Error code: " + str(msg[0]) + "Message : " + msg[1]
	sys.exit()
else:
	print "Socket Created!"

server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

try:
	server.bind(server_addr)
except socket.error, msg :
	print "Bind failed. Error code: " + str(msg[0]) + "Message : " + msg[1]
	sys.exit()
else:
	print "Socket Bind OK!"

server.listen(5)
print "Socket listening..."

while True:
	client, client_addr = server.accept()
	print "Connected by", client_addr
	while True:
		data = client.recv(BUF_SIZE)
		if data == "quit":
			print "Ready to close client"
			break;
		print data
		client.sendall(data)
		print "Send OK!"
	print "Client close"
	client.close()

print "Server close"
server.close()


