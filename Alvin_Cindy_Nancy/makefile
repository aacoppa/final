main: server.o client.o 
clean:
	rm server
	rm client
	rm *~
	rm *#
server.o: server.h server.c
	gcc -o server server.c
client.o: client.h client.c
	gcc -o client client.c