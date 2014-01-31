#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <errno.h>
#include "model.h"
#include "ui.h"
#include "map.h"
#include "logic.h"

char userQuit = 0;

int main(int argc, char **argv) {
  
  int socket_id;
  int i, b;
  
  struct sockaddr_in sock;
  
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  
  sock.sin_family = AF_INET;
  if (argc > 1)
    inet_aton( argv[1], &(sock.sin_addr) );
  else
    inet_aton("149.89.150.100", &(sock.sin_addr));
  
  sock.sin_port = htons(24645);
  
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  printf("connection made; waiting for all players\n");
  if (c < 0)
    printf("error: %s\n", strerror(errno));
  // getting terrs
  terrs = malloc(sizeof(territory)*43);
  b = read(socket_id, terrs, sizeof(territory)*43);
  b = read(socket_id, &pNum, sizeof(int));
  if (b < 0) {
    printf("Error reading:\n\t%s\n", strerror(errno));
    return 1;
  }
  printf("received map info (%dB) for p%d\n", b, pNum);
  if (b == 0)
    return 1;
  if (init_SDL())
    return 1;
  int gTurn;
  while (!userQuit) {
    if (pNum == gTurn && madeMove.destination) {
      net_move m = rtonetmove(madeMove);
      b = write( socket_id, &(m), sizeof(net_move) );
      userQuit = ui_update();
      if (!m.destination)
        read(socket_id, &gTurn, sizeof(int));
    } else if (pNum != gTurn){
      net_move move;
      b = read(socket_id, &move, sizeof(net_move));
      queue_move(nettormove(move));
      userQuit = ui_update();
      //printf("\tReceived update\n");
    } else {
      sleep(1);
    }
  }
  close(socket_id);
  cleanup_SDL();
  return 0;
}
