all:
	-mkdir ~/.rxc &> /dev/null
	make main
main: client.o game.o
	gcc game.o client_side.o main.c -o rxc 
client.o: client_side.c client_side.h
	gcc -c client_side.c
game.o:	game.c game.h
	gcc -c game.c
clean:
	-rm *.o
	-rm a.out
	-rm rxc
