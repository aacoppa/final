#include "headers.h"



int socket_id;

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

  system("clear");
  printf("Waiting for clients to connect.\nPress ENTER to start the game\n");

  fgets(buf,MAX_LEN,stdin);
  write(socket_id,"start",8);
  
  /*struct GAME_MEM *game_stats;*/
  char send_text[MAX_LEN];

  while(1){
    int b = read(socket_id,buf,sizeof(buf));
    if(b <= 0){
      close(socket_id);
      exit(0);
    } else {
      system("clear");
      if(!strcmp(buf,"exit")){
	close(socket_id);
	exit(0);

      } else if(!strcmp(buf,"end")){
	/* end game - put into idle state */
	read(socket_id,buf,sizeof(buf));
	printf("%s\nPress ENTER to start new game\n",buf);

	fgets(buf,MAX_LEN,stdin);
	printf("Read something\n");
	write(socket_id,"start",8);
      } else { 
	printf("%s\n",buf);
      }
    }
  }

  return 0;
}


