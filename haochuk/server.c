#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#define READ 0
#define WRITE 1

int A2s[2];
int A2c[2];
int B2c[2];
	

void subserver( int socket_client ); 
void subserver2( int socket_client ); 
int main() {

  int socket_id, socket_client;
  char buffer[256];
  int i, b;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  int clients = 0;
  pipe(A2s);
  pipe(A2c);
  pipe(B2c);

  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Soket file descriptor: %d\n", socket_id); 
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(24601);  
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );
  i =  listen( socket_id, 1 );
  while(1) {
    if (clients<2){
      printf("Accpeting a connection\n");
      socket_length = sizeof(server); 
      socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
      printf("accepted connection %d\n",socket_client);
      clients++;
      i = fork();
      if ( i == 0 ) {
	if(clients==1){
	  subserver(socket_client);
	  exit(0);
	}
	if(clients==2){
	  subserver2(socket_client);
	  exit(0);
	}
      }
      else 
        close(socket_client);
      printf("Waiting for new connection\n");
    }//connected all players
    else{
      memset(buffer,0,256);
      read(A2s[READ],buffer,sizeof(buffer));
      *buffer='A';
      write(A2c[WRITE],buffer,strlen(buffer));
      write(B2c[WRITE],buffer,strlen(buffer));
    }
  }//end of while

}

void subserver( int socket_client ) {

    char buffer[256];
    int b, i;
    b = 0;

    //do client stuff continuously
    while (1) {

      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer);
      
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0 )
	break;

      write(A2s[WRITE],buffer,strlen(buffer));
      read(A2c[READ],buffer,sizeof(buffer));
      write( socket_client, buffer, strlen(buffer));
    }
    
    //close this client connection
    close(socket_client);
}


void subserver2( int socket_client ) {

    char buffer[256];
    int b, i;
    b = 0;

    //do client stuff continuously
    while (1) {
      /*

      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("2 Received: %s\n", buffer);
      
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0 )
	break;

      //processand write back
      *buffer='A';
      */
      memset(buffer,0,256);
      read(B2c[READ],buffer,sizeof(buffer));
      write( socket_client, buffer, strlen(buffer));
    }
    
    //close this client connection
    close(socket_client);
}
