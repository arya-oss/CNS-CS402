#!/usr/bin/env python
'''Secure Group Chat client Implementation

  Author: Rajmani Arya
  Date: 24th Oct 2016
'''
from Crypto import Random
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import socket
import time
import hashlib
import threading
import argparse
import base64

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

def sender(conn, port):
    addr = ('127.0.0.1', port)
    while True:
        msg = raw_input('cmd $ ')
        conn.sendto(msg, addr)

def gen_key():
    rng = Random.new().read
    key = RSA.generate(1024, rng)
    return key

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Secure Group Chat Leader terminal version')
    parser.add_argument('own_port', type=int, help='Port Address of own')
    parser.add_argument('port', type=int, help='Port address of host')
    parser.add_argument('name', type=str, help='unique username')

    args = parser.parse_args()
    key = gen_key()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('127.0.0.1', args.own_port))
    sock.connect(('127.0.0.1', args.port))

    pub_key = key.publickey();
    sock.send(pub_key.exportKey('PEM'))
    r_key = sock.recv(1024)
    r_pub_key = RSA.importKey(r_key)
    md = hashlib.md5()
    md.update(args.name)
    sig = key.sign(md.hexdigest(), 16)
    sig = str(sig[0])
    l_sig = len(sig)
    if l_sig < 3:
        l_sig = '0' + str(l_sig)
    else:
        l_sig = str(l_sig)
    msg_pack = args.name+sig+l_sig
    # print msg_pack
    k = Random.new().read(16)
    aes = AESCipher(k)
    enc_msg = aes.encrypt(msg_pack)
    k_msg = r_pub_key.encrypt(k, 16)
    k_msg = ''.join(k_msg)
    sock.send(enc_msg+k_msg)
    private_key_chunks = sock.recv(1024)
    sock.close()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('127.0.0.1', args.own_port))
    t = threading.Thread(name='sender', target=sender, args=(sock, args.port))
    t.start()
    while True:
        msg, addr = sock.recvfrom(1024)
        if msg == 'need_key':
            print 'Asked for Key'
            sock.sendto(private_key_chunks, ('127.0.0.1', args.port))
        else:
            print msg
