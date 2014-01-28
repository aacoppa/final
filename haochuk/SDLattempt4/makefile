CFLAGS = -Wall -pedantic -Werror
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer
OBJS   = draw.o init.o input.o graphics.o main.o map.o player.o
PROG = 'The Haunted Mansion'
CXX = gcc

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h src/structs.h
	$(CXX) $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o
