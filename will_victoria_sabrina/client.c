#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "map.h"
#include "logic.h"
#include "model.h"

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
  
  if (init_SDL())
    return 1;
  net_move move;
  while (1) {
    if (pNum == gameTurn && move.destination) {
      b = write( socket_id, &move, sizeof(net_move) );
    } else if (pNum != gameTurn){
      //b = read
      ui_update();
      printf("\tReceived update\n");
    }
  }
  close(socket_id);
  cleanup_SDL();
  return 0;
}
