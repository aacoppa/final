#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "map.h"
#include "logic.c"
#include "model.h"

int main(int argc, char **argv) {

  int socket_id;
  int i, b;
  
  struct sockaddr_in sock;

  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  sock.sin_family = AF_INET;

  inet_aton( argv[1], &(sock.sin_addr) );
    
  sock.sin_port = htons(24601);
  
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);

  while (1) {
    b = write( socket_id, terr, sizeof(terr) );
    
    printf("\tReceived\n");
  }

  close(socket_id);
  
  return 0;
}
