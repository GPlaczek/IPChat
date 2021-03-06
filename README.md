# Chat server and client in C using ipc's
This project is an assignment for PSiW at PUT. This repository contains source code to client and server programs.

### Compilation

The make file allows for easy compilation by typing in the terminal command: 

```
make
```
It creates two executable files in bin/ directory - *client* and *serv*. 

Additionally, it has a feature that kills all message queues and currently running programs when typed into the terminal:

```
make kill
```

It is important to use this feature after exiting all programs, otherwise the chat will not work properly after restarting. Keep in mind that this command removes all IPCs in your system, so if you use any IPCs unrelated to this app, they will be removed. DON'T use this command if you want to keep other IPCs.In such case, you need to manually remove IPCs created by *serv* and *client* programs and kill any remaining processes of *serv* or *client*.

### Running

In order to connect a client to the server, you must first run the *serv* binary and then run up to 16 instances of *client* binary. After having succesfully connected to the server, client prints out a quick list of available actions that can be performed within the application.

### Directories

*Src* directory includes source codes of client and server which are the backbone of the whole project. The main loop is implemented in them.

*Lib* directory contains libraries used in project. They include declarations of functions and data structures used in main loop.

There are definitions of the above are contained in *def* directory.

### Protocol

Communication between components takes place by sending data structures, called *queries*, through message queues between the client and the server. Each of queries has its own unique number *(id)* - which is the process identifier obtained by getpid() function, *message* and *type*. The server knows how to process a particular message thanks to the type of query. The initiation of sending the first query to the server takes place when the user enters his name, then the server reads the message type and recognizes it as *ENTER_QUERY*. Next it checks whether the entered name is available and if so, it registers the user in the system. After this process, the terminal window displays instructions and a list of commands that can be used, such as joining a channel, leaving a channel, sending a private or public message, displaying a list of channels with users and leaving a chat.
After entering any command, exactly as before a query of a certain type is sent to the server, which processes the command accordingly, often using functions from serv_utils.c - e.g. joining a channel, leaving a channel, listing users etc. The client uses function defined in client_utils.c such as displaying chat instructions or getting the current time. <br/>
Structures used to manage channels:
- *User* - conatains the username and his unique id.
- *Channel* - contains channel name, user count and array of user structures.

### Authors
- Grzegorz P??aczek (148071)
- Kamil Ka??u??ny (148121)