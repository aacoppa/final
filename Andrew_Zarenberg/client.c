#include "headers.h"

void miss();
void idle();
void print_fail();
void print_full(char);

int rows = 20;
int cols = 20;

int timer;
int mycount;

int socket_id;

/* shmem */
int sd;
int *child_pid;

/* SIGUSR1:
   Because the program is waiting for user input through fgets, the timer is
   updated through signals sent every second. 
*/
static void sighandler(int signo){
  if(signo == SIGINT){
    close(socket_id);

    struct shmid_ds d;
    shmctl(sd,IPC_RMID,&d);

    system("clear");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);

  /* socket start */
  struct sockaddr_in sock;
  socket_id = socket(AF_INET,SOCK_STREAM,0);
  
  sock.sin_family = AF_INET;
  sock.sin_port = htons(SERVER_PORT);
  inet_aton(SERVER_IP,&(sock.sin_addr));

  connect(socket_id,(struct sockaddr *)&sock,sizeof(sock));
  /* socket end */
  
  char buffer[MAX_LEN];
  int b;

  /* select variables */
  fd_set fds;
  struct timeval timeout;
  int max_fd;
  int select_result;


  /* shmem */
  sd = shmget(SHM_KEY,sizeof(int), IPC_CREAT | 0666);
  printf("sd: %d\n",sd);
  child_pid = (int *)shmat(sd,NULL,0);
  *child_pid = -1;

  while(1) {

    timer = 5;

    /* get previous line from server */
    b = read( socket_id, buffer,MAX_LEN);    
    buffer[b] = 0; /* ensure there's a null at the end */

    printf("Read: %s\n",buffer);


    /* fork off to display have timer */
    if(fork() == 0){
      *child_pid = getpid();

      for(;timer>0;timer--){
	system("clear");
	int x, y;
	for(y=0;y<2;y++){
	  for(x=0;x<timer;x++) printf("### ");
	  printf("\n");
	}

	printf("\n\n\n\t\t%s = ",buffer);
	fflush(stdout);
	sleep(1);
      }

    } else {

      /* SELECT */
      timeout.tv_sec = timer;
      timeout.tv_usec = 0;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);
      max_fd = STDIN_FILENO;

      select_result = select(max_fd+1,&fds,NULL,NULL,&timeout);
      
      if(FD_ISSET(STDIN_FILENO,&fds)){
	fgets(buffer,MAX_LEN,stdin);
      } else if(select_result == 0){ 
	/* timeout */
	strcpy(buffer,"time");
      }
      kill(*child_pid,9);

    
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0 )
	break;
    
      write(socket_id, buffer, MAX_LEN);

      read(socket_id,buffer,MAX_LEN);



      if(!strcmp(buffer,"fail")){
	print_fail();
      } else {
	idle();
      }

    }
  }


  close(socket_id);

  return 0;
}



void miss(){
  char buf[MAX_LEN];
  write(socket_id,"fail",MAX_LEN);
}

void idle(){ print_full('#'); }
void print_fail(){ 
  print_full('.'); 
  sleep(1);
  idle();
}


void print_full(char a){
  system("clear");
  int x,y;
  for(x=0;x<rows;x++){
    for(y=0;y<cols;y++) printf("%c",a);
    printf("\n");
  }
}
