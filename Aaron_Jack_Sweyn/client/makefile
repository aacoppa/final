all:
	-mkdir ~/.rxc &> /dev/null
	make main
main: client.o helicopter.o
	gcc helicopter.o client_side.o main.c -o rxc -lncurses
client.o: client_side.c client_side.h
	gcc -c client_side.c
helicopter.o:	helicopter.c helicopter.h
	gcc -c helicopter.c
clean:
	-rm *.o
	-rm a.out
	-rm rxc
