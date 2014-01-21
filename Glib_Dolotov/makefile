main: main.c store.o binary_math.o
	gcc main.c store.o binary_math.o -lm
store.o: store.c store.h
	gcc -c store.c
binary_math.o: binary_math.c binary_math.h
	gcc -c binary_math.c
