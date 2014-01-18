server: database.o compose.o
	gcc -llibssl -llibcrypto -lsqlite3 database.o compose.o server_side.c
database.o:
	gcc -c database.c
compose.o: compose.c compose.h
	gcc -c compose.c
clean:
	rm *.o
	rm *.db
	rm a.out


database: compose.o
	gcc -lsqlite3 compose.o database.c
