#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "model.h"
#include "map.h"
#include "logic.c"    

void subserver( int socket_client ) {
  int n;
  int b;
  terrs = territories();
    while (1) {
     
      distribute(n);
      b = read( socket_client,terrs, sizeof(terrs));
    close(socket_client);
}

int main() {
  int socket_id, socket_client;
  int i;
  int n = 1;
    
  struct sockaddr_in server;
  socklen_t socket_length;
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;
  
  server.sin_port = htons(24601);
 
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  i =  listen( socket_id, 1 );

  while(1) {

    printf("Accpeting a connection\n");
    socket_length = sizeof(server); 
  
    if (n <= 5){
      socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
      printf("accepted connection %d\n",socket_client);
    }

    n++;

    i = fork();
    if ( i == 0 ) {
       subserver(socket_client);
     }
    else 
        close(socket_client);

    printf("Waiting for new connection\n");
  }

}
