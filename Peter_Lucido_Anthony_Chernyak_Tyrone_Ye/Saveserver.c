#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headers.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

	

int main() {

  int socket_id, socket_client;
  char buffer[256];
  int i, b;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  server.sin_family = AF_INET;

  //This is the server, so it will listen to anything coming into the host computer
  server.sin_addr.s_addr = INADDR_ANY;
  
  //set the port to listen on, htons converts the port number to network format DIFFERENT PORT
  server.sin_port = htons(24607);
  
  //bind the socket to the socket struct
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  //wait for any connection
  i =  listen( socket_id, 1 );

  //acept connections continuously
  while(1) {

    printf("Waiting for connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 

     while (1) {
    //accept the incoming connection, create a new file desciprtor for the socket to the client
    socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);

      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer);
      sleep(1);

      if ( strncmp(buffer, "save", sizeof(buffer)) == 0){
	strcpy(buffer, "send");
	write( socket_client, buffer, strlen(buffer));
	b = read( socket_client, buffer, sizeof(buffer) );
	int fd = open("savefile.file", O_RDWR | O_CREAT, 0666);
	write(fd, buffer, sizeof(buffer));
	close(fd);
	close(socket_client);
      }
      
      if ( strncmp(buffer, "load", sizeof(buffer)) == 0){
	  int fd = open("savefile.file", O_RDWR | O_CREAT, 0666);
	  read(fd, buffer, sizeof(buffer));
	  buffer[255] = "";
	  write( socket_id, buffer, strlen(buffer));   
	  close(fd);	
      }
      close(socket_client);
      printf("Waiting for connection\n");
    }


    printf("Waiting for new connection\n");
  }

}
