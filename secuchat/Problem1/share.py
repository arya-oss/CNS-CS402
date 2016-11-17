#!/usr/bin/env python

'''Secure P2P file sharing based on PKC and Symmetric Block Cipher

   Author: Rajmani arya
   Date: 24th Oct 2016
'''

import socket
from Crypto import Random
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import argparse
import base64
import threading
import time
import os
import hashlib

SHARE_DIR=None

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

def secure_recv(enc_msg, key, r_pub_key):
    '''return decpher msg and authenticated(True or False)
    :enc_msg is encrypted msg recieved from socket
    :key is private key of reciever
    :r_pub_key is public key of sender
    '''
    start_pos = len(enc_msg)-128
    pad_key = enc_msg[start_pos:]
    enc_msg = enc_msg[:start_pos]

    secret_key = key.decrypt(pad_key)

    aes = AESCipher(secret_key)
    dec_msg = aes.decrypt(enc_msg)
    pad_len = int(dec_msg[-3:])
    start_pos = len(dec_msg) - pad_len - 3
    hash_part_enc = dec_msg[start_pos:-3]
    hash_tuple = (long(hash_part_enc),)
    msg = dec_msg[:start_pos]
    md = hashlib.md5()
    md.update(msg)
    hashed_msg = md.hexdigest()
    return msg, r_pub_key.verify(hashed_msg, hash_tuple)

def secure_send(conn, data, key, r_pub_key):
    '''return void
    :conn active socket connection
    :data raw data has to be sent
    :key private key of sender
    :r_pub_key public key of reciever
    '''
    md = hashlib.md5()
    md.update(data)
    hashed_msg = md.hexdigest()
    hash_part_enc = key.sign(hashed_msg,32)
    hash_part_enc = str(hash_part_enc[0])
    l = len(hash_part_enc)
    if l < 100:
        l = '0' + str(l)
    else:
        l = str(l)
    sec_key = Random.new().read(16)

    aes = AESCipher(sec_key)
    enc_msg = aes.encrypt(data+hash_part_enc+l)
    sec_part = r_pub_key.encrypt(sec_key,32)
    sec_part = ''.join(sec_part)

    enc_msg = enc_msg + sec_part
    conn.send(enc_msg)
    time.sleep(0.5)

def gen_key():
    '''returns private key of RSA
    '''
    rng = Random.new().read
    rsa_key = RSA.generate(1024, rng)
    return rsa_key

def controller():
    print 'controller started ..'
    while True:
        cmd = raw_input('cmd $ ')
        parts = cmd.split("#")
        if len(parts) == 3 and parts[0] == "ls":
            conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                conn.connect((parts[1], int(parts[2])))
            except socket.error as e:
                print e.message
                continue
            # print 'connection established'
            key = gen_key()
            pub_key = key.publickey().exportKey('PEM')
            conn.send(pub_key)
            # print 'public key sent', pub_key
            r_pub_key = conn.recv(1024)
            # print 'recv public key', r_pub_key
            r_pub_key = RSA.importKey(r_pub_key)
            secure_send(conn, "ls", key, r_pub_key)
            data = conn.recv(1024)
            # print 'recv response'
            data, authenticated = secure_recv(data, key, r_pub_key)
            conn.close()
            if authenticated == True:
                print data
            else:
                print 'Authentication fail !'
        elif len(parts) == 4 and parts[0] == 'get':
            conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                conn.connect((parts[2], int(parts[3])))
            except socket.error as e:
                print e.message
                continue
            # print 'connection established'
            key = gen_key()
            pub_key = key.publickey().exportKey('PEM')
            conn.send(pub_key)
            # print 'public key sent', pub_key
            r_pub_key = conn.recv(1024)
            # print 'recv public key', r_pub_key
            r_pub_key = RSA.importKey(r_pub_key)
            secure_send(conn, parts[0]+"#"+parts[1], key, r_pub_key)
            # print 'send enc cmd'
            data = conn.recv(65535) # Max 65 kilobytes
            # print 'recv response'
            data, authenticated = secure_recv(data, key, r_pub_key)
            conn.close()
            if authenticated == True:
                filepath = os.path.join(SHARE_DIR, parts[1])
                with open(filepath, 'w') as f:
                    f.write(data)
                print 'File Written Success !!'
            else:
                print 'Authentication fail !'
        else:
            print '''
for message: ls#127.0.0.1#8040
for connect: get#filepath#127.0.0.1#8040
'''
def handler(conn):
    r_pub_key = conn.recv(1024)
    r_pub_key = RSA.importKey(r_pub_key)
    key = gen_key()
    public_key = key.publickey().exportKey('PEM')
    conn.send(public_key)
    # print 'send_public_key', public_key
    enc_msg = conn.recv(1024)
    # print 'recv_enc_cmd', enc_msg
    msg, authenticated = secure_recv(enc_msg, key, r_pub_key)
    if authenticated ==  True:
        parts = msg.split("#")
        if msg == 'ls':
            data = '\n'.join(os.listdir(SHARE_DIR))
            secure_send(conn, data, key, r_pub_key)
        elif parts[0] == "get":
            filepath = os.path.join(SHARE_DIR, parts[1])
            f = open(filepath, 'r')
            data = f.read()
            secure_send(conn, data, key, r_pub_key)
    else:
        print 'Authentication fail'
    conn.close()

def responder(sfd):
    print 'Server started !! ...'
    while True:
        conn, addr = sfd.accept()
        print 'Got a request from', addr
        t = threading.Thread(name='handler', target=handler, args=(conn,))
        t.start()

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Secure Chat Client terminal version')
    parser.add_argument('ip', type=str, help='IP Address of host')
    parser.add_argument('port', type=int, help='Port address of host')
    parser.add_argument('share', type=str, help='Abs Path of Shared folder')

    args = parser.parse_args()
    SHARE_DIR = args.share

    sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sfd.bind((args.ip, args.port))
    sfd.listen(5)
    
    t1 = threading.Thread(name='responder', target=responder, args=(sfd,))
    t2 = threading.Thread(name='controller', target=controller)
    
    t1.start()
    t2.start()
    t1.join()
    t2.join()
