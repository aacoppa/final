main: server.o client.o 

server.o: server.h server.c
	gcc -o server server.c

client.o: client.h client.c
	gcc -o client client.c

gui.o: gui.c gui.h maze.o
	gcc -o gui.c maze.o -lSDL	

maze.o: maze.h maze.c
	gcc -c maze.c

clean:
	rm *~
	rm *#
	rm server
	rm client
