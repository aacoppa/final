#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv){
  int socket_id;
  char buffer[256];
  int i,b;
  
  struct sockaddr_in sock;
  socket_id = socket(AF_INET,SOCK_STREAM,0);
  sock.sin_family = AF_INET;
  inet_aton(argv[1],&(sock.sin_addr)); //reads ip from command line &translates
  sock.sin_port = htons(24601); //sets port to listen to
  int c = connect(socket_id,(struct sockaddr *)&sock,sizeof(sock));
  printf("connect returned: %d\n",c);
  
  while(1){
    printf("Enter message: ");
    fgets(buffer,sizeof(buffer),stdin);
    *(strchr(buffer,'\n'))=0; // takes the return at the end to null
    b = write(socket_id,buffer,strlen(buffer)+1);
    if(strncmp(buffer,"exit",sizeof(buffer))==0)
      break;
    b = read(socket_id,buffer,strlen(buffer));
    printf("received from server: %s\n",buffer);
  }
  close(socket_id);
  return 0;
}
 
