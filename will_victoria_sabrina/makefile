LIBS=-lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
build: ui.c ui.h model.c model.h map.c map.h
	gcc ui.c model.c map.c -lm -o RISK $(LIBS)
