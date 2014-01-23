
#include "forkingserver.c"

int main() {

  int socket_id, socket_client;
  char buffer[256];
  int i, b;
  
  struct sockaddr_in server;
  socklen_t socket_length;

 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  /
  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;
  
  server.sin_port = htons(24601);
  

  i = bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  
  i =  listen( socket_id, 0 );

  while(1) {

    printf("Accpeting a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 
    int l = sizeof(server); 

    socket_client = accept(socket_id, (struct sockaddr *)&server, &l);

    printf("accepted connection %d\n",socket_client);

    while (1) {

      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer);
      
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0)
        break;


      write( socket_client, buffer, strlen(buffer));
    }
    

    close(socket_client);
    printf("Waiting for new connection\n");
  }

