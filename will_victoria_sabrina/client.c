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
#include "model.h"
#include "ui.h"
#include "map.h"
#include "logic.h"


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
  
  sock.sin_port = htons(24601);
  
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  // getting terrs
  terrs = malloc(sizeof(territory)*43);
  b = read(socket_id, terrs, sizeof(territory)*43);
  if (b < 0) {
    printf("Error reading:\n\t%s\n", strerror(errno));
    return 1;
  }
  //  printf("terrs is %d bytes long\n", b);
  if (init_SDL())
    return 1;
  net_move move;
  int gTurn;
  while (1) {
    if (pNum == gTurn && move.destination) {
      b = write( socket_id, &move, sizeof(net_move) );
    } else if (pNum != gTurn){
      //b = read
      ui_update();
      printf("\tReceived update\n");
    }
  }
  close(socket_id);
  cleanup_SDL();
  return 0;
}
