server: database.o compose.o
	gcc -lsqlite3 database.o compose.o -o server server_side.c
	#gcc -O3 server_side.c sqlite3.c -lpthread -ldl compose.o database.o
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
