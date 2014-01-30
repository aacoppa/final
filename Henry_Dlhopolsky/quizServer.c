#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct client{
  int index;
  int rightAnswers;
} client;
/*
client clients[10];
char questions[10][200];
int qnum = 0;
int fd1;
char buff[1064];
client users[20];
char parsed[20][20];
char timeout[50],accepted[50],rejected[50];

*/
int main(){
  char answers[10];
  char* questions[10];
  questions[0] = "If you needed to sort billions of integers what sorting algorithm would be most efficient?\na:Mergesort \nb: Quicksort\nc:Insertion \nd:Heap";
  answers[0] = 'c';
  questions[1] = "Who were the authors of the book 'The C programming language'?\na:Brains Kenningham and Darnell Rookster\nb:Seemour Butts and Chris P. Fry\nc:Brian Kernighan and Dennis Ritchie\nd:C. Worthy and C. Senor";
  questions[2] = "What year was google founded?\na: 1990\nb:1993,1998,1830";
  questions[3] = "What was the name of the progammer who created the GCC Compiler and the origonal Emacs?,Lou Zar,Lau Pole,Louis Charles,Richard Stallman";
  questions[4] = "What's the name of Linux's mascot?,Toledo,Trunk,Tux,Tuxe";
  questions[5] = "What was the first videogame system to offer online gaming?,NES,Playstation 1,SEGA Dreamcast,SONY Dreamcast";
  questions[6] = "How many Star Trek movies have their been?,5,7,11,13";
  questions[7] = "What is the answer to life the universe and everything?,Earth,Apple Pie,42,Dolphins";
  questions[8] = "How does one 'accomplish' the first mission of starfox 64?,saving falco, doing a barrel roll, saving slippy, trick question: you must do all of these things";
  questions[9] = "If global warming continues what commodity from spaceballs might we need on earth?,Oxygen Cans,Wookies,Space Balls,Lazer Guns";
  int socket_id, socket_client;
  char buffer[256];
  int i, b, x, n;
  struct sockaddr_in server;
  struct sockaddr_in clients[2];
  int ln = 0;
  char lc = 'l';
  socklen_t socket_length;
  socket_id = socket( AF_INET, SOCK_DGRAM, 0);
  printf("Soket file descriptor: %d\n", socket_id);

  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;
  
  //set the port to listen on, htons converts the port number to network format
  server.sin_port = htons(24601);
  
  //bind the socket to the socket struct
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );
  char login[10];
  //acept connections continuously

  socket_length = sizeof(server);
  /*
    if(qnum = 0){
      sleep(20);
      printf("waiting for connections(20 seconds)");
    }
    connectS();
    askQuestion(qnum);
    qnum++;
  */

    while(ln < 2){
      printf("going to recieve: \n");
      b = recvfrom(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&clients[ln], &socket_length);
      buffer[1]= '\0';
      printf("recieved: %s\n", buffer);
      if(ln == 0){
	sprintf(login,"l%d",ln);
	sendto(socket_id, login, sizeof(login), 0, (struct sockaddr *)&clients[ln], socket_length);
	ln++;
      } 
      else{
	if(ln == 1){
	  sprintf(login,"l%d",ln);
	  sendto(socket_id, login, sizeof(login), 0, (struct sockaddr *)&clients[ln], socket_length);
	  ln++;
	}
      }
    }
      for(x = 0; x < 11; x++){ 
	printf("sending question# %d\n",x);
	for(ln = 0; ln < 2; ln++){
	  sendto(socket_id, questions[x], strlen(questions[x])+1, 0, (struct sockaddr *)&clients[ln], socket_length);
	}
	printf("listening for answer\n");
	for(ln=0;ln<2;ln++){
	  b = recvfrom(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&clients[ln], &socket_length);
	  printf("recieved: %s\n",buffer);
	}
      }
}
/*
int connectS(){
    //sends initial login request to clients
  // sendto(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, socket_length);
    //read request response, decide if allowed to login, add information to array
    b = recvfrom(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &socket_length);
    // parsed = parse(buffer);
    printf("%s \n",buffer);
    if(strncmp(parsed[0], "login", sizeof(parsed[0])) && numclients <= 10){
      sendto(socket_id, accepted, sizeof(accepted), 0, (struct sockaddr *)&server, socket_length);
      clients[numclients].index = numclients;
    }
    else{
      sendto(socket_id, rejected, sizeof(rejected), 0, (struct sockaddr *)&server, socket_length);
    }
}

void askQuestion(string question){
  while(1){
    sendto(socket_id, questions[qnum], sizeof(questions[qnum]), 0, (struct sockaddr *)&server, socket_length);
    recvfrom(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &socket_length);
    parsed = parse(buffer);
    if(strncmp(parsed[0], "Answer", sizeof(parsed[0]) && strcmp(parsed[2], questions[numqs]->answer, sizeof(parsed[2])))){
      client[atoi(parsed[1])]->rightanswers++;
    }
    sleep(10);
    sendto(socket_id, timeout, sizeof(timeout), 0, (struct sockaddr *)&server, socket_length);
  }
  if(qnum = 11){
    
     }
*/  
   
