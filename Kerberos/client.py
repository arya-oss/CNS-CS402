#!/usr/bin/env python

import socket
import threading
import time
import argparse
from Crypto import Random
from Crypto.Cipher import AES
import base64

if __name__=='__main__':
	parser = argparse.ArgumentParser(description="Authentication Server for Kerberos Protocol")
	parser.add_argument('as_port', type=int, help="Port address for Auth Server")
	parser.add_argument('serv_port', type=int, help="Port address for Server")
	parser.add_argument('user_id', type=str, help="Your user id")
	parser.add_argument('server_id', type=str, help="Server id")
	args = parser.parse_args()

	pwd = raw_input('your password: ')
	token = args.user_id+'|'+pwd+'|'+args.server_id

	as_addr = ('127.0.0.1', args.as_port)
	serv_addr = ('127.0.0.1', args.serv_port)

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	# send token to Authentication Server
	print 'Sending to AS'
	sock.sendto(token, as_addr)
	ticket, addr = sock.recvfrom(1024)
	print 'got reply from AS'
	if addr !=  as_addr or ticket == 'auth failed!':
		print 'Wrong AS replied ! Fallback !'
		exit(1)
	time.sleep(0.500)
	print 'Sending to Server', ticket
	sock.sendto(ticket, serv_addr)
	msg, addr = sock.recvfrom(1024)
	if addr !=  serv_addr:
		print 'Wrong Server replied ! Fallback !'
		exit(1)

	print msg
	sock.close()