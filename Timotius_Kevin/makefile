main: client server Tclient Tserver

client: client.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c

Tclient: Tclient.c Tclient.h
	gcc -o Tclient Tclient.c

Tserver: Tserver.c Tserver.h
	gcc -pthread -o Tserver Tserver.c

