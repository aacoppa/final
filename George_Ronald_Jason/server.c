#include "headers.h"
#include "game.c"

int main(){
  int socket_id, socket_client;
  struct sockaddr_in server;
  socklen_t socket_length;
  int players = 0;
  
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Socket ID: %d\n", socket_id);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);
  
  int i = bind(socket_id, (struct sockaddr *)&server,sizeof(server));
  printf("Socket: %d\n",i);
  listen(socket_id,0);

  printf("Waiting for players to connect");
  socket_length = sizeof(server);
  
  if (players< MAX_PLAYERS){
    socket_client = accept(socket_id, (struct sockaddr *)&server,&socket_length);
    players++;
  }
    
  print("Player Connection Accepted\n");
  
  //gameSetup();
  
  while (1){
    
    
  }
  
  close(socket_client);
  return ;

}
