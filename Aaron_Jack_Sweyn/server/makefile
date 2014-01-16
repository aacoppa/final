database: compose.o
	gcc -lsqlite3 compose.o database.c

compose.o: compose.c compose.h
	gcc -c compose.c



clean:
	rm *.o
	rm *.db
	rm a.out
