#!/usr/bin/env python

import socket
import threading
import time
import argparse
from Crypto import Random
from Crypto.Cipher import AES
import base64

users = {
	# username: password
	'raj': '123',
	'mani': '234',
	'arya': '345'
}

servers = {
	# username : secret key
	'server1' : '\xb2\xdc[\x91#|\xf9\x97\x10\x10\x95`QZ\xfb\x10',
	'server2' : '\xfa\xa5\xef\xe8[o[\xa2\x11\xb6\x0c^\xde\xad\xaf\xdf',
	'server3' : '\x07\x00\x03\xf9\x03\xa2g\xa2\xc2\xb1f\xd0\x9e\xd4\xc3c'
}

server_addr = {
	# username : addr
	'server1' : ('127.0.0.1', 8040),
	'server2' : ('127.0.0.1', 8041),
	'server3' : ('127.0.0.1', 8042)
}

BS = 16
# for padding and unpadding raw data before and After encryption and description
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS)
unpad = lambda s : s[:-ord(s[len(s)-1:])]

class AESCipher:
    def __init__(self, key):
        self.cipher = AES.new(key)
    def encrypt(self, data):
        raw = pad(data)
        return base64.b64encode(self.cipher.encrypt(raw))

    def decrypt(self, data):
        enc = base64.b64decode(data)
        return unpad(self.cipher.decrypt(enc))

if __name__=='__main__':
	parser = argparse.ArgumentParser(description="Authentication Server for Kerberos Protocol")
	parser.add_argument('cli_port', type=int, help="Port address for client")
	parser.add_argument('serv_port', type=int, help="Port address for Server")
	args = parser.parse_args()

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind(('127.0.0.1', args.cli_port))

	sock_s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock_s.bind(('127.0.0.1', args.serv_port))

	print 'Authentication Server Started !'

	while True:
		msg, addr = sock.recvfrom(1024)
		print 'User incoming', addr
		cli_id, cli_pass, serv_id = msg.split("|")
		# if user is not in database or server is not in database or password is wrong
		if users[cli_id] is not None and servers[serv_id] is not None and users[cli_id] == cli_pass:
			aes = AESCipher(servers[serv_id])
			ticket = cli_id+'|'+addr[0]+'|'+serv_id
			ticket = aes.encrypt(ticket)
			# Send ticket to server
			ticket_s = cli_id + '|' + ticket
			sock_s.sendto(ticket_s, server_addr[serv_id])
			print 'Ticket sent to server'
			# Send ticket to client
			sock.sendto(ticket, addr)
			print 'Ticket sent to client'
		else:
			sock.sendto('auth failed!', addr)
			