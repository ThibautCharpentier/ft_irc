# ft_irc

ft_irc is a 42 school team project where we have to create our own IRC server, following Internet RFC standards. We don't have to develop a client or handle server-to-server communication.

## Status

* Success
* Grade: 110/100

## Rules

The server must be capable of handling multiple clients at the same time and never hang. Forking is not allowed. All I/O operations must be **non-blocking**.
Only **one poll() or equivalent** can be used for handling all these operations.

We must choose an IRC client. Our IRC client is **Irssi**. It must be able to connect to our server without encountering any error.
Communication between client and server has to be done via **TCP/IP**.

We have to implement the following features:
* We must be able to authenticate, set a **nickname**, a **username**, join a **channel**, send and receive private messages.
* All the messages sent from one client to a **channel** have to be forwarded to every other client that joined the **channel**.
* We must have **operators** and regular users.

We have to implements the following commands that are specific to **operators**:
* ```KICK``` - Eject a client from a channel.
* ```INVITE``` - Invite a client to a channel.
* ```TOPIC``` -  Change or view the channel topic.
* ```MODE``` - Change the channel's mode: **i** for setting/removing Invite-only channel, **t** for setting/removing the restrictions of the ```TOPIC``` command, **k** for setting/removing the channel key, **o** for giving/taking channel operator privilege and **l** for setting/removing the user limit to channel.

### Bonus

We have to create a bot which is able to connect to the server. The bot can respond to several commands if a user requests it with the ```NOTICE``` command.

## Usage

Use ```make``` command to compile then run the server with:
```
./ircserv <port> <password>
```
Example:
```
./ircserv 6667 password
```
Then, open a new prompt and use this following command to connect a client to the server:
```
irssi -c localhost -p 6667 -w password
```

### Bonus

To run the bonus part, move to the ```bonus``` folder and use ```make``` command again. Then, run the bonus server with:
```
./ircserv_bonus <port> <password>
```
Example:
```
./ircserv_bonus 6667 password
```
Then, open another prompt and move to the ```bonus/bot``` folder. Use ```make``` command and run the bot with:
```
./ircbot <name> <host> [port] [password]
```
Example:
```
./ircbot bot localhost 6667 password
```

Now, with a connected client, you can request the bot with this following command for example:
```
/notice bot !help
```
***
Made by:
* Ismérie George: <ismerie.george@gmail.com>
* Thibaut Charpentier: <thibaut.charpentier42@gmail.com>
