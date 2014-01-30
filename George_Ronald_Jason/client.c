#include "headers.h"

int main(int argc, char **argv){

  int socket_id;
  struct sockaddr_in sock;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);
  sock.sin_family = AF_INET;
  inet_aton(argv[1], &(sock.sin_addr) );
  sock.sin_port = htons(SERVER_PORT);
  
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connected: %d\n", c);

  while(1){

  }
}
