#include "headers.h"



int socket_id;
int sd;

static void sighandler(int signo){
  if(signo == SIGINT){
    close(socket_id);
    exit(0);
  }
}

int main(){
  signal(SIGINT,sighandler);

  /* socket start */
  struct sockaddr_in sock;
  socket_id = socket(AF_INET,SOCK_STREAM,0);
  
  sock.sin_family = AF_INET;
  sock.sin_port = htons(SERVER_PORT);
  inet_aton(SERVER_IP,&(sock.sin_addr));

  connect(socket_id,(struct sockaddr *)&sock,sizeof(sock));

  char buf[8];
  write(socket_id,"hello",8);
  read(socket_id,buf,sizeof(buf));
  /* socket end */

  struct GAME_MEM game_stats;

  while(1){
    read(socket_id,buf,sizeof(buf));
    if(!strcmp(buf,"exit")) exit(0);
    else {
      read(socket_id,&game_stats,sizeof(struct GAME_MEM));

      system("clear");
      printf("Score: %d\nLives: %d\n",game_stats.score,game_stats.lives);
    }
  }

  return 0;
}

