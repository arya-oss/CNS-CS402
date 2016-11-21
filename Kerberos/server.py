#!/usr/bin/env python

import socket
import threading
import time
import argparse
from Crypto import Random
from Crypto.Cipher import AES
import base64

my_id = 'server1'
secret_key = '\xb2\xdc[\x91#|\xf9\x97\x10\x10\x95`QZ\xfb\x10'

auth_users = {}

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

aes = AESCipher(secret_key)

def auth_server(sock):
	while True:
		msg, addr = sock.recvfrom(1024)
		print 'Req from AS', msg
		cli_id, ticket = msg.split("|")
		token = aes.decrypt(ticket)
		client_id, client_addr, server_id = token.split("|")
		if cli_id == client_id and server_id == my_id:
			auth_users[client_id] = client_addr
			print 'client added name', client_id
		else:
			print 'Wrong token from AS'

def auth_client(sock):
	while True:
		ticket, addr = sock.recvfrom(1024)
		print 'Req from client', ticket
		token = aes.decrypt(ticket)
		client_id, client_addr, server_id = token.split("|")
		# user is not present or server id is wrong or client addr is different
		if auth_users[client_id] is not None and server_id == my_id and auth_users[client_id] == addr[0]:
			# client authenticated
			sock.sendto('Congrats ! You understood Kerberos Protocol', addr)
		else:
			sock.sendto('unidentified user', addr)
			print 'Wrong token from client'

if __name__=='__main__':
	parser = argparse.ArgumentParser(description="End Server")
	parser.add_argument('cli_port', type=int, help="Port address for client")
	parser.add_argument('serv_port', type=int, help="Port address for Auth Server")
	args = parser.parse_args()

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind(('127.0.0.1', args.cli_port))

	sock_s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock_s.bind(('127.0.0.1', args.serv_port))

	print 'Secure Server Started !'

	t = threading.Thread(name='auth server', target=auth_server, args=(sock_s,))
	t.start()
	s = threading.Thread(name='auth client', target=auth_client, args=(sock,))
	s.start()
	s.join()
	t.join()

