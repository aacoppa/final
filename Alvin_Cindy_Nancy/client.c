#include "client.h"

int main(int argc, char **argv) {
  int socket_id;
  char buffer[256];
  int i, b;
  
  //Set up socket and connect to server specified in arguments
  struct sockaddr_in sock;
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Socket file descriptor: %d\n", socket_id);
  sock.sin_family = AF_INET;
  inet_aton( argv[1], &(sock.sin_addr) );
  sock.sin_port = htons(24601);
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);

  //do client stuff continuously
  while (1) {
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    *(strchr(buffer, '\n')) = 0;
    
    b = write( socket_id, buffer, strlen(buffer) + 1 );
    
    if ( strncmp(buffer, "exit", sizeof(buffer)) == 0)
      break;
    
    b = read( socket_id, buffer, strlen(buffer));
    
    printf("\tReceived: %s\n", buffer);
  }
  
  close(socket_id);
  
  return 0;
}
