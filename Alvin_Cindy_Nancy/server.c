#include "server.h"

char * capitalize(char *s) {
  char *p;
  p = s;
  while ( *p ) {
    if ( *p >= 'a' && *p <= 'z' )
      *p = *p - ('a' - 'A');
    p = p + 1;
  }
  return s;
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

      //TODO: CHANGE TO "ROOM" CODE
      capitalize( buffer );

      write( socket_client, buffer, strlen(buffer));
    }
    
    //close this client connection
    close(socket_client);
}
        

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
  
  //set the port to listen on, htons converts the port number to network format
  server.sin_port = htons(24601);
  
  //bind the socket to the socket struct
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  //wait for any connection
  i =  listen( socket_id, 1 );

  //acept connections continuously
  while(1) {

    printf("Accpeting a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 

    //accept the incoming connection, create a new file desciprtor for the socket to the client
    socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
    printf("accepted connection %d\n",socket_client);

    i = fork();
    if ( i == 0 ) {
       subserver(socket_client);
     }
    else 
        close(socket_client);


    printf("Waiting for new connection\n");
  }

}
