# Remember to switch build to the top once networking works
game: ui.c ui.h model.c model.h map.c map.h logic.c logic.h
	gcc ui.c model.c map.c logic.c -lSDL2 -lSDL2_ttf -lm -o RISK

build:  game server client

server: forkingserver.c
	gcc forkingserver.c

client: client.c
	gcc client.c
