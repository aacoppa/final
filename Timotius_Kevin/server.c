#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
char *white[600];
char *black[200];

void subserver( int socket_client ) {

    char buffer[256];
    char s[256];
    char n[1];
    int b, i, c;
    b = 0;
    memset(buffer,0,strlen(buffer));
    //do client stuff continuously
    while (1) {
      //give cards to client
      b = read(socket_client,n,sizeof(buffer));
      if(strcmp(n,"0") == 0)
	for(c = 0;c < 5;c++){
	  strcat(buffer,white[rand()%537]);
	  strcat(buffer,",");
	}
      printf("%s\n",buffer);
      write(socket_client,buffer,strlen(buffer));
      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer); 
      if ( strncmp(buffer, "exit", sizeof(buffer)) == 0 )
	break;

      write( socket_client, buffer, strlen(buffer));
    }
    
    //close this client connection
    close(socket_client);
}
	

int main() {

  int socket_id, socket_client;
  char buffer[417];
  int i, b;
  int fdw,fdb;
  char *s;
  char cardsw[12000];
  char cardsb[10000];
  char** temp;
  int bytesRead = 0;
  int counter = 0;
  //reading white card
  fdw = open("white.txt",O_RDONLY,0664);
  bytesRead = read(fdw,cardsw,sizeof(cardsw));
  temp = white;
  char *line1 = cardsw;
  while(s = strsep(&line1,",")){
    *temp = s;
    *temp++;
  }
  close(fdw);
  //print white
  /*
  while(white[counter] != NULL){
    printf("\twhite cards[%d]: %s\n",counter,white[counter]);
    counter++;
  }
  */
  fdb = open("black.txt",O_RDONLY,0664);
  bytesRead = read(fdb,cardsb,sizeof(cardsb));
  temp = black;
  char *line2 = cardsb;
  while(s = strsep(&line2,",")){
    *temp = s;
    *temp++;
  }
  close(fdb);
  //print black
  /*
  counter = 0;
  while(black[counter] != NULL){
    printf("\tblack cards[%d]: %s\n",counter,white[counter]);
    counter++;
  }
  */

  //NETWORK
  struct sockaddr_in server;
  socklen_t socket_length;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  server.sin_family = AF_INET;

  //This is the server, so it will listen to anything coming into the host computer
  server.sin_addr.s_addr = INADDR_ANY;
  
  //set the port to listen on, htons converts the port number to network format
  server.sin_port = htons(24601);
  
  //bind the socket to the socket struct
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  //wait for any connection
  i =  listen( socket_id, 1 );

  //acept connections continuously
  while(1) {

    printf("Accepting a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 

    //accept the incoming connection, create a new file desciprtor for the socket to the client
    socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
    printf("accepted connection %d\n",socket_client);

    i = fork();
    if ( i == 0 ) {
       subserver(socket_client);
     }
    else 
        close(socket_client);


    printf("Waiting for new connection\n");
  }

}
