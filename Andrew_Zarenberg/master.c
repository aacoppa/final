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

  char buf[MAX_LEN];

  /* socket end */

  printf("Waiting for clients to connect.  Press ENTER to start the game\n");

  fgets(buf,MAX_LEN,stdin);
  write(socket_id,"start",8);
  
  printf("Starting...\n");

  /*struct GAME_MEM *game_stats;*/
  char send_text[MAX_LEN];

  while(1){
    read(socket_id,buf,sizeof(buf));
    system("clear");
    if(!strcmp(buf,"exit")) exit(0);
    else printf("%s\n",buf);
  }

  return 0;
}

