# TP RESEAU : IRC

## About the Project

This project implements a working IRC server and client.  (+ basic TCP server and client)

### Developped in :
* C 

## Getting Started

#### the project contains 4 files :

- tcplient.c

- tcpserver.c

- ircclient.c

- ircserver.c


#### List of IRC Commands:
- get a nickname unregistered ```/nickname pseudo``` (implemented) 
- get a nickname registered ```/nickname pseudo password```(implemented)
- send broacast alert ```/alerte message```(implemented)
- send unicast alert ``/alerte pseudo message``(implemented)
- send private message ``/mp pseudo message``(implemented)
- get server time ``/date``(implemented)
- register a nickname ``/register nickname password``(implemented)
- unregister a nickname ``/unregister pseudo password ``(not implemented)
- send file ``/send pseudo ./file.txt``(implemented) ==> for example ``/send pseudo sendFiles/filesend.txt`` and received in receivedFiles Folder.
- exit ``/exit``(implemented)


### OTHER usefull commands :
- compile all files ```make all```
- Clear All Files : ```make cleanall```
- Lunch ircclient : ```./ircclient 127.0.0.1 8081```
- Lunch ircserver : ```./ircserver```
- Lunch tcpclient : ```./tcpclient```
- Lunch tcpserver : ```./tcpserver```


## Credits :

Created by Julien SANCHEZ

