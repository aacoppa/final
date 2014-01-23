#include "forkingserver.c"

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
  i = bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  //wait for any connection
  i =  listen( socket_id, 0 );

  //acept connections continuously
  while(1) {

    printf("Accpeting a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 
    int l = sizeof(server); 

    //accept the incoming connection, create a new file desciprtor for the socket to the client
    socket_client = accept(socket_id, (struct sockaddr *)&server, &l);

    printf("accepted connection %d\n",socket_client);

    //do client stuff continuously
    while (1) {

      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer);
      
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0)
        break;

      //processand write back
      capitalize( buffer );

      write( socket_client, buffer, strlen(buffer));
    }
    
    //close this client connection
    close(socket_client);
    printf("Waiting for new connection\n");
  }

