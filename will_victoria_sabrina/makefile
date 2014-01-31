build: server game

# Remember to switch build to the top once networking works
game: ui.c ui.h model.c model.h map.c map.h logic.c logic.h
	gcc client.c ui.c model.c map.c logic.c -lSDL2 -lSDL2_ttf -lm $(CFLAGS) -o RISK

server: forkingserver.c logic.c map.c
	gcc logic.c map.c forkingserver.c $(CFLAGS) -o server
