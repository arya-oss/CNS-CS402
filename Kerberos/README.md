# Kerberos Protocol

### How To Run

```bash
	Write each command in a new terminal

	# ./AS.py port_for_client port_for_own
	$ ./AS.py 8030 8050

	# ./server.py port_for_client port_for_auth_server
	$ ./server.py 8020 8040
	
	# ./client.py auth_server_port server_port user_id server_id (password is 123 for user raj)
	$ ./client.py 8030 8020 raj server1

```
#### In an unprotected network environment,

Any client can apply to any server for service. The obvious security risk is that of
impersonation. An opponent can pretend to be another client and obtain
unauthorized privileges on server machines. To counter this threat, servers must be
able to confirm the identities of clients who request service. Each server can be
required to undertake this task for each client/server interaction, but in an open
environment, this places a substantial burden on each server.
An alternative is to use an authentication server (AS) that knows the pass-
words of all users and stores these in a centralized database. In addition, the AS
shares a unique secret key with each server. These keys have been distributed
physically or in some other secure manner.

```
(1) C :AS -> IDC | PC | IDV
(2) AS :C -> Ticket
(3) C :V -> IDC | Ticket
Ticket= E(Kv ,[IDC |ADC | IDV ])

where
C = client
AS = authentication server
V = server
IDC = identifier of user on C
IDV = identifier of V
PC = password of user on C
ADC = network address of C
Kv = secret encryption key shared by AS and V

```
Remember that the security of the Kerberos server should not automatically be assumed but must be
guarded carefully.

In this scenario, the user logs on to a workstation and requests access to server V.
The client module C in the user’s workstation requests the user’s password and
then sends a message to the AS that includes the user’s ID, the server’s ID, and the
user’s password. The AS checks its database to see if the user has supplied the
proper password for this user ID and whether this user is permitted access to
server V. If both tests are passed, the AS accepts the user as authentic and must
now convince the server that this user is authentic. To do so, the AS creates a ticket
that contains the user’s ID and network address and the server’s ID. This ticket is
encrypted using the secret key shared by the AS and this server. This ticket is then
sent back to C. Because the ticket is encrypted, it cannot be altered by C or by an
opponent.

With this ticket, C can now apply to V for service. C sends a message to V con-
taining C’s ID and the ticket. V decrypts the ticket and verifies that the user ID in
the ticket is the same as the unencrypted user ID in the message. If these two match,
the server considers the user authenticated and grants the requested service.
Each of the ingredients of message (3) is significant. The ticket is encrypted to
prevent alteration or forgery. The server’s ID (ID V ) is included in the ticket so that
the server can verify that it has decrypted the ticket properly. ID C is included in the
ticket to indicate that this ticket has been issued on behalf of C. Finally, AD C serves
to counter the following threat. An opponent could capture the ticket transmitted in
message (2), then use the name ID C and transmit a message of form (3) from
another workstation. The server would receive a valid ticket that matches the user
ID and grant access to the user on that other workstation. To prevent this attack, the
AS includes in the ticket the network address from which the original request came.
Now the ticket is valid only if it is transmitted from the same workstation that ini-
tially requested the ticket.