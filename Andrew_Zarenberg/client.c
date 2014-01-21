#include "headers.h"

void miss();
void idle();
void print_fail();
void print_full(char);
void end_process();

int rows = 53;
int cols = 134;

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
    end_process();
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

  struct SERVER_TO_CLIENT *game;


  /* shmem */
  sd = shmget(SHM_KEY,sizeof(int), IPC_CREAT | 0666);
  printf("sd: %d\n",sd);
  child_pid = (int *)shmat(sd,NULL,0);
  *child_pid = -1;

  idle();

  while(1) {

    /* get previous line from server */
    b = read( socket_id, buffer,MAX_LEN);

    if(b <= 0){
      end_process();
    }

    if(!strcmp(buffer,"end")){
      print_fail();
    } else if(!strcmp(buffer,"idle")){
      idle();
    } else {

    read(socket_id,&timer,sizeof(int));



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
	sleep(2);
	idle();
      } else {
	idle();
      }

    }
  }

  }

  close(socket_id);

  return 0;
}


/* Function idle():
   Arguments: None
   Returns:   N/A

   Purpose: Displays the computer in the "idle" state
*/
void idle(){ 
  system("clear");
  int x,y;
  for(x=0;x<rows;x++){
    for(y=0;y<cols;y++) printf("#");
    printf("\n");
  }
}


/* Function print_fail():
   Arguments: None
   Returns:   N/A

   Purpose: Displays a large X to alert the user they were wrong
*/
void print_fail(){
  system("clear");

  int x,y,first;

  int size = 10,move=cols/rows;

  for(x=1;x<rows;x++){
    first = abs(x*move-(rows*move)/2);
    for(y=0;y<(rows*move)/2-first;y++) printf(" ");
    for(y=0;y<size;y++) printf("@");
    if(first > size){
      for(y=0;y<first*2-size;y++) printf(" ");
      for(y=0;y<size;y++) printf("@");
    } else {
      for(y=0;y<first*2;y++) printf("@");
    }
    printf("\n");
  }
}

/* Function end_process():
   Arguments: None
   Returns:   N/A

   Purpose: Ends the client program
*/
void end_process(){
  close(socket_id);

  struct shmid_ds d;
  shmctl(sd,IPC_RMID,&d);

  system("clear");
  exit(0);
}
