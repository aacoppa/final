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


void create_shmem();
void delete_shmem();
int create_sem();
void delete_sem();
void math_problem(char[8],int *,int);


int count; /* how many times user has made it*/
int start_game; /* boolean value, signal to start game */

struct GAME_MEM *game;
int master_socket, socket_id;


static void sighandler(int signo){
  
  if(signo == SIGINT){
    write(master_socket,"exit",8);

    int x;
    for(x=0;x<computer_len;x++){
      /*      write(computer[0],"exit",8);*/
      close(computer[0]);
    }

    delete_sem();

    shmdt(game);
    delete_shmem();
    close(master_socket);
    close(socket_id);

    exit(0);
  }
}




int main(){
  signal(SIGINT, sighandler);

  start_game = 0;

  /*create_shmem();*/

  lives = 5;
  score = 0;
  timer = 10;

  int semd = create_sem();
  /* semaphore*/
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;



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
  

  /*
    int f = fork();
    if(f == 0){

      while(1){

	sleep(1);

	strcpy(stuff,"");
	math_answer = -1;

	sb.sem_op = -1;
	semop(semd,&sb,1);

	printf("In: %d\n",socket_client);

	math_problem(math_string,&math_answer,10);

	write(socket_client,math_string,MAX_LEN);
	read(socket_client,stuff,MAX_LEN);


	if(math_answer == atoi(stuff)){
	  write(socket_client,"idle",MAX_LEN);
	  game->score++;
	} else {
	  write(socket_client,"fail",MAX_LEN);
	  game->lives--;
	}

	sprintf(send_text,"\n\n\n\n\t\tScore: %d\n\n\t\tLives: %d\n",game->score,game->lives);

	write(master_socket,send_text,sizeof(send_text));

	sb.sem_op = 1;
	semop(semd,&sb,1);
	  
	printf("Out: %d\n",socket_client);

      }
      exit(0);
      close(socket_client);
    }
    

    
    }*/
  
  close(socket_id);
  end_process();


  return 0;
}



int create_sem(){
  delete_sem();
  int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
  if(semd > 0){
    printf("Semaphore created successfully! [%d]\n",semd);
  } else {
    printf("Error creating semaphore. [%d]\n",semd);
  }
  
  union semun su;
  su.val = 1;
  semctl(semd, 0, SETVAL, su);

  return semd;
}

void delete_sem(){
  int semd = semget(SEM_KEY, 1, 0666);
  semctl(semd, 0, IPC_RMID);
}
/*
void create_shmem(){
  int sd = shmget(SHM_KEY, sizeof(struct GAME_MEM), IPC_CREAT | 0666);
  if(sd > 0){
    printf("Shared memory created successfully! [%d]\n",sd);
  } else {
    printf("Error creating shared memory. [%d]\n",sd);
  }
   
  game = (struct GAME_MEM *)shmat(sd,NULL,0);
  game->lives = DEFAULT_LIVES;
  game->score = 0;
  }*/

void delete_shmem(){
  int sd = shmget(SHM_KEY, sizeof(int), 0666);
  
  struct shmid_ds d;
  shmctl(sd,IPC_RMID,&d);
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


  


