#include "headers.h"


union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};


int score;
int lives;

int computer[34];
int computer_len;
int last_computer = -1;

int timer;


void math_problem(char[8],int *,int);


int count; /* how many times user has made it*/
int start_game; /* boolean value, signal to start game */

struct GAME_MEM *game;
int master_socket, socket_id;


static void sighandler(int signo){
  
  if(signo == SIGINT){
    write(master_socket,"exit",8);

    int x;
    /*    for(x=0;x<computer_len;x++){*/
    for(x=computer_len-1;x>=0;x--){
      close(computer[x]);
    }

    close(master_socket);
    close(socket_id);

    exit(0);
  }
}




int main(){
  signal(SIGINT, sighandler);

  start_game = 0;

  lives = 5;
  score = 0;
  timer = 10;


  char stuff[MAX_LEN];

  /* socket stuff */
  int socket_client;
  struct sockaddr_in server;
  socklen_t socket_length;

  socket_id = socket(AF_INET,SOCK_STREAM,0);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);

  int i = bind(socket_id,(struct sockaddr *)&server,sizeof(server));
  printf("Socket: %d\n",i);
  listen(socket_id,0);
  /* socket end */

  char math_string[MAX_LEN];
  int math_answer;


  /* socket connect to master */
  printf("\nPlease run ./master on another computer.\n");
  socket_length = sizeof(server);
  master_socket = accept(socket_id,(struct sockaddr *)&server,&socket_length);
  printf("Accepted\n");


  printf("\nConnected to master.  Run ./client on all computers you wish to be part of this game.\n");

  char send_text[MAX_LEN];



  while(1){
    /* use select to break out of it when master starts the game */
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(master_socket,&fds);
    FD_SET(socket_id,&fds); 
    select(master_socket+1,&fds,NULL,NULL,NULL);

    if(FD_ISSET(socket_id,&fds)){

      /* socket */
      socket_length = sizeof(server);
      socket_client = accept(socket_id,(struct sockaddr *)&server,&socket_length);
      /* socket end */
	
      computer[computer_len++] = socket_client;
    
      printf("Accepted connection to %d\n",socket_client);
    } else if(FD_ISSET(master_socket,&fds)){
      break;
    }
  }


  sprintf(send_text,"\n\n\n\n\t\tScore: %d\n\n\t\tLives: %d\n",score,lives);

  write(master_socket,send_text,sizeof(send_text));


  while(1){
    sleep(1);
    if(computer_len > 1){
      srand(time(NULL));

      /* Make sure it's not the same computer twice in a row */
      while(socket_client == last_computer){
	socket_client = computer[rand()%computer_len];
      }
      last_computer = socket_client;

      strcpy(stuff,"");
      math_answer = -1;


      math_problem(math_string,&math_answer,10);

      printf("Writing to socket: %d\nSent: %s\n",socket_client,math_string);

      write(socket_client,math_string,MAX_LEN);
      int b = read(socket_client,stuff,MAX_LEN);

      printf("Read from socket: %d\nReceived: %s\nSize: %d\n",socket_client,stuff,b);

      if(math_answer == atoi(stuff)){
	write(socket_client,"idle",MAX_LEN);
	score++;
      } else {
	write(socket_client,"fail",MAX_LEN);
	lives--;
      }
      sprintf(send_text,"\n\n\n\n\t\tScore: %d\n\n\t\tLives: %d\n",score,lives);

      write(master_socket,send_text,sizeof(send_text));

    }
  }
  
  
  close(socket_id);
  end_process();


  return 0;
}



/* math problem*/
void math_problem(char math_string[8], int *answer, int max){
  srand(time(NULL));
  int op = rand()%4;
  int n1,n2;

  if(op == 0){ 
    /* addition */
    n1 = rand()%(max*2)+2;
    n2 = rand()%(max*2)+2;
    sprintf(math_string,"%d + %d",n1,n2);
    *answer = n1+n2;
  
  } else if(op == 1){
    /* subtraction
       answer will ALWAYS be positive
    */
    n1 = rand()%(max*2)+2;
    n2 = rand()%(max*2)+2;

    if(n1 > n2){
      sprintf(math_string,"%d - %d",n1,n2);
      *answer = n1-n2;
    } else {
      sprintf(math_string,"%d - %d",n2,n1);
      *answer = n2-n1;
    }
  
  } else if(op == 2){
    /* multiplication */
    n1 = rand()%max+2;
    n2 = rand()%max+2;
    
    sprintf(math_string,"%d x %d",n1,n2);
    *answer = n1*n2;

  } else if(op == 3){
    /* division
       answer will ALWAYS be an integer
    */
    n1 = rand()%(max*2)+4;
    n2 = n1/2-4;
    if(n2 < 2) n2 = 2;
    
    /* check if n1 is prime number */
    int x,y = 0;
    while(y == 0){
      for(x=2;x*x<n1;x++){
	if(n1%x == 0){
	  y = 1;
	  break;
	}
      }
      if(y == 0) n1++;
    }

    /* check if n1 is divisible by n2 */
    while(n1%n2 != 0){
      n2++;
    }

    sprintf(math_string,"%d / %d",n1,n2);
    *answer = n1/n2;
  }
}


  


