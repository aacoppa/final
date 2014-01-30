main: server.o client.o maze.o

clean:
	rm server
	rm client
	rm *~
	rm *#
server.o: server.h
	gcc -o server server.c maze.c
client.o: client.h client.c
	gcc -o client client.c
