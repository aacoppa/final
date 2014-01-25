#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void subserver(int socket_client);
int main() {
  int socket_id, socket_client;
  char buffer[256];
  int i,b;

  struct sockaddr_in server;
  socklen_t socket_length;

  socket_id = socket(AF_INET, SOCK_STREAM,0);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(24601); // port number to listen to

  i = bind(socket_id,(struct sockaddr *)&server, sizeof(server));
  i = listen(socket_id,1);  
  while(1){
    printf("waiting for a connection\n");
    socket_length =sizeof(server);    
    socket_client =accept(socket_id,(struct sockaddr *)&server, &socket_length);
    printf("accepted connection %d\n", socket_client);
    i = fork();
    if (i==0)
      subserver(socket_client);
    else
      close(socket_client);
  }//end of while
}

void subserver(int socket_client){
  char buffer[256];
  int i,b;
  b = 0;

  while(1){
    b = read(socket_client,buffer,sizeof(buffer));
    printf("received: %s\n",buffer);
    if(strncmp(buffer,"exit",sizeof(buffer))==0)
      break;
    *buffer = 'A';
    write(socket_client,buffer,strlen(buffer));
  }
  close(socket_client);
}
  
