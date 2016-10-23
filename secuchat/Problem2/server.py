#!/usr/bin/env python
'''Secure Chat Host part implementation

    Auhtor: Rajmani Arya
    Date: 20th Oct 2016
'''

import socket
from Crypto import Random
from Crypto.Cipher import AES
from Crypto.PublicKey import RSA
import argparse
import base64
import threading

BS = 32
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

def sender(conn, key):
    aes = AESCipher(key)
    while True:
        msg = raw_input()
        enc_msg = aes.encrypt(msg)
        try:
            conn.send(enc_msg)
        except socket.error as err:
            print err.message
            continue
        if msg == "END":
            conn.close()
            print "Closing chat..."
            break

def reciever(conn, key):
    aes = AESCipher(key)
    while True:
        try:
            enc_msg = conn.recv(1024)
        except socket.error as err:
            print err.message
            continue
        msg = aes.decrypt(enc_msg)
        if msg == "END":
            conn.close()
            break
        print 'User:', msg

parser = argparse.ArgumentParser(description='Secure Chat Host terminal version')
parser.add_argument('ip', type=str, help='IP Address to host')
parser.add_argument('port', type=int, help='Port address to host')

args = parser.parse_args()

rng = Random.new().read

rsa_key = RSA.generate(1024, rng)
public_key = rsa_key.publickey()

host_msg = public_key.exportKey('PEM')

sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sfd.bind((args.ip, args.port))
sfd.listen(5)

print 'Server started', args.ip, args.port
while True:
    conn, addr = sfd.accept()
    print 'client connected', addr
    # send public key after connection established
    conn.send(host_msg)
    # recv client public key in PEM format
    sock_msg = conn.recv(1024)
    cli_key = RSA.importKey(sock_msg)
    symm_key = raw_input('Enter 16 bytes secret key: ')
    enc_symm_key = base64.b64encode(''.join(cli_key.encrypt(symm_key,32)))
    conn.send(enc_symm_key)

    s = threading.Thread(name='sender', target=sender, args=(conn, symm_key))
    r = threading.Thread(name='reciever', target=reciever, args=(conn, symm_key))
    s.start()
    r.start()
    s.join()
    r.join()

    sfd.close()
    break
