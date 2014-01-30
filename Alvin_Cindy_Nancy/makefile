main: server.o client.o 
clean:
	rm *~
	rm *#
	rm server
	rm client
server.o: server.h server.c
	gcc -o server server.c
client.o: client.h client.c
	gcc -o client client.c