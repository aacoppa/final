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
  char n[1];
  int i, b;
  char* hand[5];
  int num = 0;
  int holder;

  
  struct sockaddr_in sock;
  
  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  
  printf("Socket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  sock.sin_family = AF_INET;
  
  //Client will connect to address in argv[1], need to translate that IP address to binary
  inet_aton(argv[1], &(sock.sin_addr) );
  
  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24601);
  
  //connect to the server
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);
  
  //do client stuff continuously
  while (1) {
    //get white cards
    sprintf(buffer,"%d",num);
    b = write(socket_id,buffer,strlen(buffer) + 1);
    b = read(socket_id,buffer,sizeof(buffer));
    num = 5;
    printf("\tReceived: %s\n",buffer);
    char* line = buffer;

    for(holder = 0;hand[holder] == NULL;holder++){
      hand[holder] = strsep(&line,",");
    }
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    *(strchr(buffer, '\n')) = 0;
    if ( strncmp(buffer, "exit", sizeof(buffer)) == 0)
      break;
    b = write(socket_id,buffer,strlen(buffer) + 1);
  }
  
  close(socket_id);
  
  return 0;
}
