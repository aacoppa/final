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

int main(){
  char* answers[10];
  char* questions[10];
  //QUESTIONS AND ANSWERS, SPOILER ALERT
  questions[0] = "If you needed to sort billions of integers what sorting algorithm would be most efficient?\na:Mergesort \nb: Quicksort\nc:Insertion \nd:Heap";
  answers[0] = "c";
  questions[1] = "Who were the authors of the book 'The C programming language'?\na:Brains Kenningham and Darnell Rookster\nb:Seemour Butts and Chris P. Fry\nc:Brian Kernighan and Dennis Ritchie\nd:C. Worthy and C. Senor";
  answers[1] = "c";
  questions[2] = "What year was google founded?\na: 1990\nb:1993\nc:1998\nd:1830";
  answers[2] = "c";
  questions[3] = "What was the name of the progammer who created the GCC Compiler and the origonal Emacs?\na:Lou Zar\nb:Lau Pole\nc:Louis Charles\nd:Richard Stallman";
  answers[3] = "d";
  questions[4] = "What's the name of Linux's mascot?\na:Toledo\nb:Trunk\nc:Tux\nd:Tuxe";
  answers[4] = "c";
  questions[5] = "What was the first videogame system to offer online gaming?\na:NES\nb:Playstation 1\nc:SEGA Dreamcast\nd:SONY Dreamcast";
  answers[5] = "c";
  questions[6] = "How many Star Trek movies have their been?\na:5\nb:7\nc:11\nd:13";
  answers[6] = "c";
  questions[7] = "What is the answer to life the universe and everything?\na:Earth\nb:Apple Pie\nc:42\nd:Dolphins";
  answers[7] = "c";
  questions[8] = "How does one 'accomplish' the first mission of starfox 64?\na:saving falco\nb:doing a barrel roll\nc:saving slippy\nd:trick question: you must do all of these things";
  answers[8] = "a";
  questions[9] = "If global warming continues what commodity from spaceballs might we need on earth?\na:Oxygen Cans\nb:Wookies\nc:Space Balls\nd:Lazer Guns";
  answers[9] = "a";
  int socket_id, socket_client;
  char buffer[256];
  int i, b, x, n;
  struct sockaddr_in server;
  struct sockaddr_in clients[2];
  int ln = 0;
  int p[2];
  p[0] = 0;
  p[1] = 0;
  char str[2][20];
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
  //by using for loops and &clients[ln], I was able to speak individually to specific clients while residing within a UDP server
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
    //Here we ask and recieve answer to the questions over the UDP server.
      for(x = 0; x < 10; x++){ 
	printf("sending question# %d\n",x);
	for(ln = 0; ln < 2; ln++){
	  sendto(socket_id, questions[x], strlen(questions[x])+1, 0, (struct sockaddr *)&clients[ln], socket_length);
	}
	printf("listening for answer\n");
	for(ln=0;ln<2;ln++){
	  b = recvfrom(socket_id, buffer, sizeof(buffer), 0, (struct sockaddr *)&clients[ln], &socket_length);
	  if(strncmp(buffer,answers[x],sizeof(buffer))){
	    p[ln] = p[ln] + 1;
	  }
	  printf("recieved: %s\n",buffer);
	}
      }
      for(ln = 0; ln < 2; ln++){
	sprintf(str[ln],"%d",p[ln]);
	sendto(socket_id, str[ln], strlen(str[ln])+1, 0, (struct sockaddr *)&clients[ln], socket_length);
	
      }
      return 1;
}
