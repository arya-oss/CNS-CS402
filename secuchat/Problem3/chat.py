#!/usr/bin/env python

'''Secure Chat implementation
    Auhtor: Rajmani Arya
    Date: 20th Oct 2016
'''

import socket
from Crypto import Random
from Crypto.PublicKey import RSA
import argparse
import base64
import threading
import time

public_keys = {}
private_keys = {}
connection = {}

def reciever(r_name):
    conn = connection[r_name]
    key = private_keys[r_name]
    print 'Chat Started with', r_name
    if conn is None or key is None:
        print 'error occured !'
        return
    while True:
        try:
            enc_msg = conn.recv(1024)
        except socket.error as err:
            print err.message
            break
        enc_msg = base64.b64decode(enc_msg)
        msg = key.decrypt(enc_msg)
        print r_name, msg
        if msg == "END":
            time.sleep(0.5)
            conn.close()
            del public_keys[r_name]
            del connection[r_name]
            del private_keys[r_name]
            print 'Chat disconnected with', r_name
            break

def gen_key():
    rng = Random.new().read
    rsa_key = RSA.generate(1024, rng)
    # public_key = rsa_key.publickey()
    # sock_msg = public_key.exportKey('PEM')
    return rsa_key

def controller(name):
    print 'controller started !!',name
    while True:
        cmd = raw_input('cmd $ ')
        parts = cmd.split("#")
        if parts[0] == "list":
            for user in connection.iterkeys():
                print user
            print len(connection), 'Active users ..'
        elif len(parts) == 3 and parts[0] == 'connect':
            sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            try:
                sfd.connect((parts[1], int(parts[2])))
            except socket.error:
                print parts[1], 'connection error !'
                continue
            key = gen_key()
            public_key = key.publickey().exportKey('PEM')
            sfd.send(name+'#'+public_key)
            data = sfd.recv(1024)
            r_name, r_key = data.split('#')
            r_pub_key = RSA.importKey(r_key)
            # threading.Lock().acquire()
            private_keys[r_name] = key
            public_keys[r_name] = r_pub_key
            connection[r_name] = sfd
            # threading.Lock().release()
            t = threading.Thread(name=r_name, target=reciever, args=(r_name,))
            t.start()
            # t.join()
        elif len(parts) == 3 and parts[0] == 'send':
            if public_keys[parts[1]] is None or connection[parts[1]] is None:
                print '{0} does not exist'.format(parts[1])
                continue
            key = public_keys[parts[1]]
            conn = connection[parts[1]]
            enc_msg = key.encrypt(parts[2], 32)
            enc_msg = base64.b64encode(''.join(enc_msg))
            print 'Send msg to', parts[1], parts[2]
            try:
                conn.send(enc_msg)
            except socket.error as err:
                print err.message
            if parts[2] == "END":
                time.sleep(0.5)
                conn.close()
                # threading.Lock().acquire()
                del public_keys[parts[1]]
                del connection[parts[1]]
                del private_keys[parts[1]]
                # threading.Lock().release()
                print "Closing chat...", parts[1]
        else:
            print '''
for message: send#sender_name#message
for connect: connect#IP_Address#Port
'''

def responder(sfd, name):
    print 'Server started !! ...', name
    while True:
        conn, addr = sfd.accept()
        print 'Got a request from', addr
        data = conn.recv(1024)
        r_name, r_key = data.split('#')
        if connection.has_key(r_name):
            print r_name,'already there !!'
            continue
        r_pub_key = RSA.importKey(r_key)
        key = gen_key()
        public_key = key.publickey().exportKey('PEM')
        conn.send(name+'#'+public_key)
        # # threading.Lock().acquire()
        private_keys[r_name] = key
        public_keys[r_name] = r_pub_key
        connection[r_name] = conn
        # threading.Lock().release()
        t = threading.Thread(name=r_name, target=reciever, args=(r_name,))
        t.start()
        # t.join()

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='Secure Chat Client terminal version')
    parser.add_argument('ip', type=str, help='IP Address of host')
    parser.add_argument('port', type=int, help='Port address of host')
    parser.add_argument('name', type=str, help='user unique name')

    args = parser.parse_args()

    sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sfd.bind((args.ip, args.port))
    sfd.listen(5)
    t1 = threading.Thread(name='responder', target=responder, args=(sfd,args.name))
    t2 = threading.Thread(name='controller', target=controller, args=(args.name,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
