#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>



int main(int argc, char **argv) {
  char todo[200];
  int socket_id;
  char buffer[256];
  int i, b, x;
  int numquestions = 0;
  int score = 0;
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
    sleep(5);
    //This section mirrors the sections of the Server function, accepting answers and transfering them to the server.
    for(x = 0; x < 10; x++){ 
      printf("recieveing question# %d\n",x);
      
      b = recvfrom(socket_id, buffer, sizeof(buffer), 0, NULL, NULL);
      printf("%s\n",buffer);
      printf("please enter answer:\n");
      fgets(buffer, sizeof(buffer), stdin);
      *(strchr(buffer, '\n')) = 0;
      sendto(socket_id, buffer, sizeof(buffer), 0, NULL, 0);
      printf("sending answer\n");
    }
    if(x = 10){
      printf("tabulating scores....\n");
      b = recvfrom(socket_id, buffer, sizeof(buffer), 0, NULL, NULL);
      printf("your score is %s! Good Job!\n",buffer);
    }
    // if(strncmp(todo, "l", 1)){
    //  retval = 1;}
    
    // else{ 
    //   return
}
  

