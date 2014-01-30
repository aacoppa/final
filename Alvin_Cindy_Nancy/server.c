#include "server.h"

int main() {
  int socket_id, socket_client;
  char buffer[256];
  int i, b;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  //Set up and bind socket
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Soket file descriptor: %d\n", socket_id);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(24601);
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );
  i =  listen( socket_id, 1 );

  while(1) {
    printf("Accepting a connection\n");
    socket_length = sizeof(server); 
    socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
    printf("accepted connection %d\n",socket_client);

    i = fork();
    if ( i == 0 )
       subserver(socket_client);
    else 
        close(socket_client);


    printf("Waiting for new connection\n");
  }
  return 1;
}

void subserver( int socket_client ) {
  char buffer[256];
  int b, i;
  b = 0;
  
  //do client stuff continuously
  while (1) {
    b = read( socket_client, buffer, sizeof(buffer) );
    printf("Received: %s\n", buffer);
    
    if ( strncmp(buffer, "exit", sizeof(buffer)) == 0 )
      break;
    
    updateScore(buffer);
    strncpy(buffer,getMaze(MAZE_WIDTH,MAZE_HEIGHT),sizeof(buffer));
    
    write( socket_client, buffer, strlen(buffer));
  }
  
  close(socket_client);
}

char* getMaze(int width,int height){
  char* maze;
  width = width*2 + 3;
  height = height*2 + 3;
  
  maze=(char*)malloc(width*height*sizeof(char));
  GenerateMaze(maze,width,height);
  ShowMaze(maze,width,height);
  return maze;
}
int updateScore(){
  return -1;
}
