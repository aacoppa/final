INCLUDES = -Isrc/
DIRS = parser net
OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c) $(foreach d, $(DIRS), $(wildcard src/$(d)/*.c)))

main: dirs $(OBJS)
	gcc -o tst.out $(OBJS)

obj/parser/%.o: src/parser/%.c src/parser/%.h
	gcc -c -o $@ $< $(INCLUDES)

obj/net/%.o: src/net/%.c src/net/%.h
	gcc -c -o $@ $< $(INCLUDES)

obj/%.o: src/%.c src/%.h
	gcc -c -o $@ $< $(INCLUDES)

dirs:
	@test -d obj || mkdir obj
	@for DIRECTORY in $(DIRS) ; do \
		test -d obj/$$DIRECTORY || mkdir obj/$$DIRECTORY; \
	done

clean:
	rm -r cv.out $(OBJS)

run: main
	./tst.out
