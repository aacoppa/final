#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {

  int socket_id;
  char buffer[256];
  int i, b;
  char * engine[2] = {"./execute", (char *)0};


  struct sockaddr_in sock;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  sock.sin_family = AF_INET;
  //Client will connect to address in argv[1], need to translate that IP address to binary
  inet_aton( argv[1], &(sock.sin_addr) );

  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24601);
  //connect to the server
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Game Copy Verified\n");
  
  close(socket_id);

  sleep(2);

  execvp("./execute", engine);

  return 0;
}
