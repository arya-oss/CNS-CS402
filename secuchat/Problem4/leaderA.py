#!/usr/bin/env python
'''Secure Group Chat Server Implementation

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

LIMIT = 1
connection = {}
rev_connection = {}
private_key_chunks = []
recv_pr_chunk = [ None for i in range(LIMIT)]

r_pub_key = None

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

def gen_key():
    rng = Random.new().read
    key = RSA.generate(1024, rng)
    return key

def authenticate(conn, key_chunk):
    key = gen_key()
    r_key = conn.recv(1024)
    r_pub_key = RSA.importKey(r_key)
    pub_key = key.publickey().exportKey('PEM')
    conn.send(pub_key)
    enc_msg = conn.recv(1024)
    conn.send(key_chunk)
    enc_k_part = enc_msg[-128:]
    enc_msg = enc_msg[:-128]
    k = key.decrypt(enc_k_part)
    # print k
    aes = AESCipher(k)
    enc_msg = aes.decrypt(enc_msg)
    # print enc_msg
    pad_len = int(enc_msg[-3:])
    start_pos = len(enc_msg)-pad_len-3
    msg = enc_msg[:start_pos]
    enc_hash_msg = (long(enc_msg[start_pos:-3]),)
    md = hashlib.md5()
    md.update(msg)
    return msg, r_pub_key.verify(md.hexdigest(), enc_hash_msg)

def controller(port, pub_key, leader_port):
    print 'Server for own clients started ..'
    conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    conn.bind(('127.0.0.1', port))
    opp_leader = ('127.0.0.1', leader_port)

    x = [str(i) for i in range(LIMIT)]
    while True:
        msg, addr = conn.recvfrom(1024)
        # print msg
        if msg[0] in x:
            print 'private key chunk recieved'
            recv_pr_chunk[int(msg[0])] = msg[1:]
        elif len(msg):
            name = connection[addr[1]]
            enc_msg = r_pub_key.encrypt(name+'#'+msg, 32)
            enc_msg = ''.join(enc_msg)
            conn.sendto(enc_msg, opp_leader)

def ask_for_key(conn):
    for addr in rev_connection.itervalues():
        conn.sendto('need_key', addr)
    print 'requested key'

def controller_recv(port, pub_key, leader_port):
    print 'Server for Leaders started..'
    conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    conn.bind(('127.0.0.1', port))
    opp_leader = ('127.0.0.1', leader_port)
    time.sleep(5)
    conn.sendto(pub_key.exportKey('PEM'), opp_leader)
    print 'Pub Key Sent !!'
    global r_pub_key
    r_pub_key, addr = conn.recvfrom(1024)
    r_pub_key = RSA.importKey(r_pub_key)
    print 'Pub Key recieved'
    while True:
        enc_msg, addr = conn.recvfrom(1024)
        ask_for_key(conn)
        time.sleep(1)
        pr_key = "".join(recv_pr_chunk)
        # print pr_key
        key = RSA.importKey(pr_key)
        msg = key.decrypt(enc_msg)
        msg_chunks = msg.split("#")
        print msg_chunks
        if len(msg_chunks) == 3:
            addr = rev_connection[msg_chunks[1]]
            print addr
            conn.sendto(msg_chunks[0]+': '+msg_chunks[2], addr)
            print 'message from {} to {}'.format(msg_chunks[0], msg_chunks[1])

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Secure Group Chat Leader terminal version')
    parser.add_argument('opp_port', type=int, help='Port Address for opponent')
    parser.add_argument('port', type=int, help='Port address of host')
    parser.add_argument('leader_port', type=int, help='Port address of opponent')

    args = parser.parse_args()
    key = gen_key()

    private_key_pem = key.exportKey('PEM')
    pub_key = key.publickey()

    chunk_size = len(private_key_pem)/LIMIT
    for i in range(LIMIT):
        private_key_chunks.append(str(i)+private_key_pem[:chunk_size])
        private_key_pem = private_key_pem[chunk_size:]
    private_key_chunks[LIMIT-1] += private_key_pem

    sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sfd.bind(('127.0.0.1', args.port))
    sfd.listen(5)
    client_index=0
    t1 = threading.Thread(name='controller_recv', target=controller_recv, args=(args.opp_port,pub_key, args.leader_port))
    t1.start()
    print 'Server Started'
    while client_index!=LIMIT:
        conn, addr = sfd.accept()
        print 'Got a request from', addr
        if len(connection) == LIMIT:
            print 'User Exceeded !!'
            continue
        name, authenticated = authenticate(conn, private_key_chunks[client_index])
        conn.close()
        if authenticated:
            connection[addr[1]] = name
            rev_connection[name] = addr
            client_index += 1
        else:
            print 'Authentication Failed !!'
    sfd.close()
    t = threading.Thread(name='controller', target=controller, args=(args.port, pub_key, args.leader_port))
    t.start()
    t.join()
    t1.join()
