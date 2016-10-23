## Cryptography and Network Security assignment

**Tools Used**

1. __Ubuntu OS__
2. __Python 2.7__
3. __Python Libraries__

    1. [socket](https://docs.python.org/2/library/socket.html)
    2. [PyCrypto](https://www.dlitz.net/software/pycrypto/)
    3. [base64](https://docs.python.org/2/library/base64.html)
    4. [threading](https://docs.python.org/2/library/threading.html)
    5. [time](https://docs.python.org/2/library/time.html)

### Problem Statments
1.  [Problem1](/secuchat/Problem1) A network in which each node can act as client or server for the other computers in the network, allowing shared access to varoius resources as files, peripherals, etc without need of central server.
    1.  Design anc code security association/agreement between server and client.
    2.  Procedure for key management
    3.  Design and code for authentication between server and client vice-versa.

2.  [Problem2](/secuchat/Problem2) Consider an RSA crypto system. Server and clients have their public and private keys. They communicate using a symmetric key and that key is distributed to client by server after connection using the public key of client.
    1.  Perform symmetric key exchange between server and client.
    2.  Data transfer between Server and client using above symmetric key.

3.  [Problem3](/secuchat/Problem3) Implement a communication system with following details
    1.  The users are divided logically into groups.
    2.  Each users can have many private and public key pairs.
    3.  Each users maintains a table in which public key is stored.
    4.  Each users maintains private keys in another table along with the id of the public key which is in encrypted format.
    5.  The encryption and decryption is based on public key id sent along with the cipher text.

    *Use any Public Key Cryptography for encryption and decryption*  

4.  [Problem4](/secuchat/Problem4) Write a socket program for secure communication between two groups `A` and `B`:
    1.  A server is connected to `n` number of registered users.
    2.  The users are divided into two groups such as `A` and `B`.
    3.  Members can be added and removed dynamically.
    4.  The communication is between two group leaders (assume the leaders are already elected).
    5.  Each group leader authenticates his members by using any authentication technique before any communication happens.
    6.  The server generates a common (public key, private key) for each group and divides the private key into shares and dispatches to the users of respectives groups.
    7.  A user from group `A` can communicate to group `B` user through the leader and vice-versa.
    8.  The encryption and decryption is using key pair (public key, private key), which is the common practice.

### Note
```bash
    # Install dependancies
    sudo apt install python-dev python-pip python-virtualenv
    virtualenv crypto
    source crypto/bin/activate
    # Install PyCrypto
    pip install pycrypto
```
