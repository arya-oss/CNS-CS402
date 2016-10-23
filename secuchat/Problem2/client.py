#!/usr/bin/env python
'''Secure Chat client part implementation

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

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Secure Chat Client terminal version')
    parser.add_argument('ip', type=str, help='IP Address of host')
    parser.add_argument('port', type=int, help='Port address of host')

    args = parser.parse_args()
    rng = Random.new().read
    rsa_key = RSA.generate(1024, rng)
    public_key = rsa_key.publickey()
    pub_key = public_key.exportKey('PEM')

    sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sfd.connect((args.ip, args.port))
    # recv host public key in PEM format
    sock_msg = sfd.recv(1024)
    host_key = RSA.importKey(sock_msg)
    # send public key after connection established
    sfd.send(pub_key)
    enc_symm_key = sfd.recv(1024)

    symm_key = rsa_key.decrypt(base64.b64decode(enc_symm_key))
    s = threading.Thread(name='sender', target=sender, args=(sfd, symm_key))
    r = threading.Thread(name='reciever', target=reciever, args=(sfd, symm_key))
    s.start()
    r.start()
    s.join()
    r.join()
