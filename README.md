# Chat server and client in C using ipc's
This project is an assignment for PSiW at PUT. In this file, you will find a technical documentation of this project (soon : - )

### Compilation

The make file allows for easy compilation by typing in the terminal command: 

```
make
```
It creates two executable files in bin/ directory - *client* and *serv*. 

Additionally, it has a feauture that kills all message queues and currently running programs when typed into the terminal:

```
make kill
```
It is important to use this feature after exiting all programs, otherwise the chat will not work properly after restarting.

### Directories

*Src* directory includes source codes of client and server which are the backbone of the whole project. The main loop is implemented in them.

*Lib* directory contains libraries used in project. They include declarations of functions and data structures used in main loop.

There are definitions of the above are contained in *def* directory.

### Protocol

Communication between components takes place by sending data structures, called *queries*, through message queues between the client and the server. Each of queries has its own unique number *(id)* - which is the process identifier obtained by getpid() function, *message* and *type*. The server knows how to process a particular message thanks to the type of query. The initiation of sending the first query to the server takes place when the user enters his name, then the server reads the message type and recognizes it as *ENTER_QUERY*. Next it checks whether the entered name is available and if so, it registers the user in the system. After this process, the terminal window displays instructions and a list of commands that can be used, such as joining a channel, leaving a channel, sending a private or public message, displaying a list of channels with users and leaving a chat. 
