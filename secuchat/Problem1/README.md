## Secure P2P File Sharing program based on PKC encryption and authentication

**public shared directory is `/home/user/Public`**

*How It Works*

Whenever any user write commands it creates a new socket connection and exchanges public keys.
now seeder either `list` or `get` file from connected peers.

__Sending__

before sending any command seeder first generates a symmetric key `K` and calculates `md5` hash of msg or text `H = md5(M)`
and sign with its own private key then it combines with original message `M` and again encrypted with `AES` cipher
with key `K` now `X = AES(M+sign(md5(M)))`. `K` is encrypted with peers public key and combines with X and final sending message is `X+E(K)`.

__Recieving__
after recieving cipher text peer separates `X` and `E(K)` and calculates `K = private_Key_E(E(K))`. After that `X` is decrypted with AES Cipher of key `K` now `D = AES(X)`. Now from `D` separate actual message `M` and signed msg `H = E(md5(M))`. `H` is verified with public key of seeder. if `verify(E(md5(M))) == md5(M)` then it ensures that this message came from original source.

## How To Run this Program
1. Setup the environment
2. First Client runs on port 8040:
    `$ ./sharing.py 127.0.0.1. 8040`
3. Second Client runs on port 8041:
    `$ ./sharing.py 127.0.0.1 8041`
4. To get a file
    `(cmd $) get#filepath#127.0.0.1#8040`
5. To see list of shared files
    `(cmd $) ls#127.0.0.1#8040`

>If secrecy of the message is desired, encryption can be combined with the digital
signature, providing privacy as well as proof of authorship. To do this, we can pick a
random key, K, with which to encrypt the message (using a symmetric algorithm). This
key will then be encrypted with the public key of each recipient of the message. A
recipient will be able to decrypt K with his private key, then decrypt the message,
compute the hash, decrypt the hash attached to the message (decrypted with the sender's
public key), and verify the origin and authenticity of the message. Let's look at this again:

>Alice has a message, M, to send to Bob. She computes its hash, H(M) and encrypts it with
her own private key: E a (H(M)) . This is her signature. Secrecy of the message is important
in this example, so she will encrypt the signed message with a symmetric algorithm
using a randomly generated key, K. The encrypted signed message is E K ({M,E a (H(M))}).
Now she has to enable only Bob to be able to decrypt this message, so she encrypts the
key, K, with Bob’s public key: E B (K). Finally, she sends out the complete message: {
E K ({M,E a (H(M))}), E B (K) }.

>When Bob gets this message. He first decrypts the key, K, using his private key. Now,
using K, he can decrypt the entire message with signature. Having done this, he
computes a hash of M, H(M). He then decrypts Alice’s signature using Alice’s public key
and compares the two hashes to validate the message.

## To Exit this application
```bash
    # CTRL-C is not going to end this application
    # to do that first do CTRL-Z and then
    ps | grep python | cut -f1 -d" " | xargs kill -9
```
