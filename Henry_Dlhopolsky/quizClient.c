#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>



int main(int argc, char **argv) {
  char ret[20][20];
  char todo[200];
  int socket_id;
  char buffer[256];
  char a[256][256];
  int i, b, x;
  char parsed[20][20];
  int numquestions = 0;
  char time[200];
  char s = 'l';
  struct sockaddr_in sock;
  
  socket_id = socket( AF_INET, SOCK_DGRAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  sock.sin_family = AF_INET;

  inet_aton( "127.0.0.1", &(sock.sin_addr) );

  sock.sin_port = htons(24601);

  connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  
  //connect to the server
  /*
  if(connectC(todo) && numquestions <= 10){
    //answer question if logged in
    AnswerQuestions();
  }
  else{
    return;
  }
  */ 
    printf("about to send \n");
    sendto(socket_id, &s, sizeof(s), 0, NULL, 0);
    printf("sent\n");
    b = recvfrom(socket_id, todo, sizeof(todo), 0, NULL, NULL);
    todo[b] = '\0';
    printf("Recieved: %s \n", todo);
    sleep(10);
    for(x = 0; x < 11; x++){ 
      printf("recieveing question# %d\n",x);
      
      b = recvfrom(socket_id, buffer, sizeof(buffer), 0, NULL, NULL);
      printf("%s\n",buffer);
      printf("please enter answer:\n");
      fgets(buffer, sizeof(buffer), stdin);
      *(strchr(buffer, '\n')) = 0;
      sendto(socket_id, buffer, sizeof(buffer), 0, NULL, 0);
      printf("sending answer\n");
    }
    // if(strncmp(todo, "l", 1)){
    //  retval = 1;}
    
    // else{ 
    //   return
}

/*
  int answerQuestion(){
    b = recvfrom(socket_id, todo, sizeof(todo), 0, NULL, NULL);
    parsed = parse(buffer);
    if(strncmp(parsed[0], "Question", sizeof(parsed[0]))){
      printf("Question: %s \n a: %s \n b: %s \n c: %s \n d: %s \n",a[1],a[2],a[3],a[4],a[5]);
      printf("Enter Answer: \n");
      fgets(buffer, sizeof(buffer), stdin);
      sendto(socket_id, buffer, sizeof(buffer), 0, NULL, 0);
      numquestions++;
    }
    if(strncmp(parsed[0], "Time", sizeof(parsed[0]))){
      printf("Time Out, please wait for the next question \n");
      sendto(socket_id, time, sizeof(time), 0, NULL, 0);
      numquestions++;
    }
  }
}
*/
  
  

