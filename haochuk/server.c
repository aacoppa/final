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
int B2s[2];
int C2s[2];
int C2c[2];
int D2s[2];
int D2c[2];
int E2s[2];
int E2c[2];

	
void subserver( int socket_client,int X2s[2],int X2c[2]); 
int main() {

  int socket_id, socket_client;
  float buffer[10];
  int i, b;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  int clients = 0;
  pipe(A2s);
  pipe(A2c);
  pipe(B2c);
  pipe(B2s);
  pipe(C2s);
  pipe(C2c);
  pipe(D2s);
  pipe(D2c);
  pipe(E2s);
  pipe(E2c);

  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Soket file descriptor: %d\n", socket_id); 
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(24601);  
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );
  i =  listen( socket_id, 1 );
  printf("accepting conenctions\n");
  while(1) {
    if (clients<2){
      printf("waiting for connection\n");
      socket_length = sizeof(server); 
      socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
      clients++;
      i = fork();
      if ( i == 0 ) {
	switch(clients){
	case 1:
	  subserver(socket_client,A2s,A2c);
	case 2:
	  subserver(socket_client,B2s,B2c);
	case 3:
	  subserver(socket_client,C2s,C2c);
	case 4:
	  subserver(socket_client,D2s,D2c);
	case 5:
	  subserver(socket_client,E2s,E2c);
	}
	exit(0);
      }
      else 
        close(socket_client);
    }//connected all players
    else{
      	switch(clients){
	case 5:
	  read(A2s[READ],buffer,sizeof(float)*2);
	  read(B2s[READ],buffer+2,sizeof(float)*2);
	  read(C2s[READ],buffer+4,sizeof(float)*2);
	  read(D2s[READ],buffer+6,sizeof(float)*2);
	  read(E2s[READ],buffer+8,sizeof(float)*2);
	  write(A2c[WRITE],buffer,sizeof(buffer));
	  write(B2c[WRITE],buffer,sizeof(buffer));
	  write(C2c[WRITE],buffer,sizeof(buffer));
	  write(D2c[WRITE],buffer,sizeof(buffer));
	  write(E2c[WRITE],buffer,sizeof(buffer));
	case 4:
	  read(A2s[READ],buffer,sizeof(float)*2);
	  read(B2s[READ],buffer+2,sizeof(float)*2);
	  read(C2s[READ],buffer+4,sizeof(float)*2);
	  read(D2s[READ],buffer+6,sizeof(float)*2);
	  write(A2c[WRITE],buffer,sizeof(buffer));
	  write(B2c[WRITE],buffer,sizeof(buffer));
	  write(C2c[WRITE],buffer,sizeof(buffer));
	  write(D2c[WRITE],buffer,sizeof(buffer));
	case 3:
	  read(A2s[READ],buffer,sizeof(float)*2);
	  read(B2s[READ],buffer+2,sizeof(float)*2);
	  read(C2s[READ],buffer+4,sizeof(float)*2);
	  write(A2c[WRITE],buffer,sizeof(buffer));
	  write(A2c[WRITE],buffer,sizeof(buffer));
	  write(B2c[WRITE],buffer,sizeof(buffer));
	case 2:
	  read(A2s[READ],buffer,sizeof(float)*2);
	  read(B2s[READ],buffer+2,sizeof(float)*2);
	  write(A2c[WRITE],buffer,sizeof(buffer));
	  write(B2c[WRITE],buffer,sizeof(buffer));
	}

    }
  }//end of while

}

/*
void subserver( int socket_client ) {

  //    char buffer[256];
  float buffer[4];
  int b, i;
  b = 0;
  
  //do client stuff continuously
  while (1) {
    
    //read from the client
    b = read( socket_client, buffer, sizeof(buffer) );
    printf("Received: %f\n", buffer[0]);
    
    if ( strncmp(buffer,"exit", sizeof(buffer)) == 0 )
      break;
    
    //    write(A2s[WRITE],buffer,strlen(buffer));
    write(A2s[WRITE],buffer,sizeof(float)*2);
    read(A2c[READ],buffer,sizeof(buffer));
    //    write( socket_client, buffer, strlen(buffer));
    write( socket_client, buffer, sizeof(buffer));
    }
    
    //close this client connection
    close(socket_client);
}
*/

void subserver( int socket_client,int X2s[2],int X2c[2]) {
  float buffer[10];  
  int b,i,j;
  b = 0;
  while (1) {
    b = read( socket_client, buffer, sizeof(buffer) );
    /*
    printf("this is what what received\n\n");
    for(j = 0;j<10;j++)
      printf("buffer[%d]: %f\n",j,buffer[j]);
    */
    write(X2s[WRITE],buffer,sizeof(float)*2);
    read(X2c[READ],buffer,sizeof(buffer));
    write( socket_client, buffer, sizeof(buffer));
    }
    close(socket_client);
}
