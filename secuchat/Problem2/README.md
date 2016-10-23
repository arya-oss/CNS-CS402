## Secure Chat
**How It Works**

After getting socket connection established. server and client exchanges their Public Key
and then using PKC it share 128-bit `AES` symmetric key, send and recieve data based on
symmetric key AES-128 Cipher.

*base64 is used to wrap the encrypted data (sometimes binary data got disturbed in socket connection)*

## How To Run this Program
1. Setup the environment

2. First Server runs on port 8040:
    `$ ./server.py 127.0.0.1. 8040`
3. Client connects on port 8040:
    `$ ./client.py 127.0.0.1 8040`

## To Exit this application
Send message `END`
or
```bash
    # CTRL-C is not going to end this application
    # to do that first do CTRL-Z and then
    ps | grep python | cut -f1 -d" " | xargs kill -9
```
