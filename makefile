all: ircclient ircserver tcpclient tcpserver

ircclient: ircclient.c
	gcc -Wall -o irc_client ircclient.c

ircserver: ircserver.c
	gcc -Wall -o irc_server ircserver.c

tcpclient: tcpclient.c
	gcc -Wall -o tcp_client tcpclient.c

tcpserver: tcpserver.c
	gcc -Wall -o tcp_server tcpserver.c

cleanall:
	 rm -f irc_client irc_server tcp_client tcp_server
