#include "Tclient.h"
int main(int argc, char *argv[]){
  int i;
  int sockfd, rw;
  struct addrinfo flags;
  struct addrinfo *server_info;

  char * hand[5];
  for (i = 0; i < 5; i++){
    hand[i] = (char *)calloc(1, BUFFER_SIZE);
  }
  char ** temp;
  char *s;
  char *buffer; 
  char * question;

  memset(&flags, 0, sizeof(flags)); // Clear so we're not working with garbage
  flags.ai_family = AF_UNSPEC; // IPv4 or IPv6 doesn't matter
  flags.ai_socktype = SOCK_STREAM; // TCP
  flags.ai_flags = AI_PASSIVE; // get the IP for me

  if (getaddrinfo(argv[1], "24601", &flags, &server_info) < 0) { // Resolve host based on CMD args
    perror("Couldn't find host");
    exit(-1);
  }

  sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol); // Initialize socket

  if (connect(sockfd, server_info->ai_addr, server_info->ai_addrlen) < 0)
    {
      perror("Couldn't connect...");
      exit(-1);
    } 
  
  printf("Welcome to cards against humanity\n");

  buffer = (char * )malloc(256);
  rw = read(sockfd, buffer, BUFFER_SIZE); // accepting first 5 cards
  printf("%s\n", buffer);

  buffer[strlen(buffer)-1] = 0;

 
  temp = hand;
  while(s = strsep(&buffer, ",")){
    *temp = s;
    *temp++;
  }
  
  /*
  for(i = 0; i < 5; i++){
    printf("\tChoice [%d]: %s\n",i,hand[i]);
    }*/
  
  free(buffer);
  while(1){
    buffer = (char *)malloc(BUFFER_SIZE);
    question = (char *)malloc(BUFFER_SIZE);
    rw = write(sockfd, "1", 1);
      
    printf("\n\nCurrent Question: \n\n");
    rw = read(sockfd, question, BUFFER_SIZE);
    
    printf("%s\n\n", question);
  
    printf("Choose a response: \n");
    
    for(i = 0; i < 5; i++){
      printf("\tChoice [%d]: %s\n",i,hand[i]);
    }
    
    bzero(buffer, BUFFER_SIZE);
    i = getchar() - 48;
    while(i < 0 || i > 4){
      printf("Please select 0, 1, 2, 3 or 4\n\n");
      i = getchar() - 48;
    }
    s = hand[i];
    printf("You picked: %d %s\n\n", i, s);
    strcpy(buffer, s);

    rw = write(sockfd, buffer, BUFFER_SIZE); // Sending the response 

    rw = read(sockfd, buffer, BUFFER_SIZE);
    printf("new choice: %s in slot %d\n", buffer, i);
    printf("bytes read %d\n", rw);
    
    strncpy(hand[i],buffer, strlen(buffer));
    printf("%s\n", buffer);
    bzero(buffer, BUFFER_SIZE);
    free(buffer);
    
    printf("press enter for next round\n");
    int i = getchar();
  }
  close(sockfd);
  return 0;

}
