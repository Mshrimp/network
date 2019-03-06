#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import socket

BUF_SIZE = 1024

ip = '127.0.0.1'
port = 8888

#server_addr = ('127.0.0.1', 8888)
server_addr = (ip, port)

try:
	client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg :
	print "Create socket failed. Error code: " + str(msg[0]) + "Message : " + msg[1]
	sys.exit()

client.connect(server_addr)

while True:
	data = raw_input("Please input string: ")
	if not data :
		print "Input can't empty, pleast input again: "
		continue
	if data == "quit":
		client.sendall(data)
		break
	client.sendall(data)
	data = client.recv(BUF_SIZE)
	print data
client.close()

