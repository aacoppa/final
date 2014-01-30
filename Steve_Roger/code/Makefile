main: game.c game.h util.o
	gcc -Wall -o game game.c util.o

util.o: util.h util.c
	gcc -Wall -c util.c

run: main
	./game
