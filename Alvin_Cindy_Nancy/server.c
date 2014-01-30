#include "server.h"
#include "maze.h"

int sem_scores,shm_scores;
int* highscore;
int i, b;  
void sighandler(int signum){
  if(signum == SIGINT){
    struct shmid_ds d;
    shmctl(shm_scores,IPC_RMID,&d);
    semctl(sem_scores,0,IPC_RMID);
    exit(0);
  }
}
int main() {
  int socket_id, socket_client;
  char buffer[256];

  struct sockaddr_in server;
  socklen_t socket_length;

  //Set up IPC for highscore
  sem_scores = semget(SEM_KEY,1,IPC_CREAT | IPC_EXCL | 0644);
  shm_scores = shmget(SHM_KEY,sizeof(int), IPC_CREAT | 0644);
  highscore = (int*) shmat(shm_scores,NULL,0);
  *highscore = 0;

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
    
    //updateScore(buffer);
    printf("Getting maze\n");
    strncpy(buffer,getMaze(buffer,MAZE_WIDTH,MAZE_HEIGHT),sizeof(buffer));
    printf("buffer:%s\n",buffer);
    write( socket_client, buffer, strlen(buffer));
    strncpy(buffer,"",sizeof(buffer));
  }
  shmdt(highscore);
  close(socket_client);
}

char* getMaze(char buffer[256],int width,int height){
  width = width*2 + 3;
  height = height*2 + 3;
  
  generateMaze(buffer,width,height);
  return buffer;
}
int updateScore(char* buffer){
  return -1;
}
