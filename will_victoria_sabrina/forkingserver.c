#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "model.h"
#include "map.h"
#include "logic.h"

int moveKey = 273;
int turnKey = 274;
int procNextKey = 275;
int memdMove;
int memdTurn;
int memdProcNext;
net_move *move;
int *turn;
int *unsentMoves;
int totalPlayers;

void subserver( int socket_client, int pNum ) {
  int bytesRead;
  printf("subserver started for p%d, awaiting further connections\n", pNum);
  (*unsentMoves)++;
  while (*unsentMoves < totalPlayers) {
    sleep(5);
  }
  int b = write(socket_client, terrs, sizeof(territory)*43);
  //printf("terrs is %lu B, wrote %d bytes\n", sizeof(territory)*43, b);
  b = write(socket_client, &pNum, sizeof(int));
  *unsentMoves = 0;
  while (1) {
    if (pNum == *turn && !unsentMoves) {
      bytesRead = read( socket_client, move, sizeof(net_move));
      *unsentMoves = totalPlayers;
      // check if turn complete
      if (!move->destination) {
        if (*turn >= totalPlayers)
          *turn = 1;
        else
          (*turn) ++;
      }
    } else if (pNum != *turn){
#warning fix
      write(socket_client, move, sizeof(net_move));
      (*unsentMoves)--;
    }
  }
  shmdt(move);
  shmdt(turn);
  shmdt(unsentMoves);
  close(socket_client);
}

int main() {
  int socket_id, socket_client;
  int i;
  int n = 1;
  // shmem
  memdMove = shmget( moveKey, sizeof(net_move), IPC_CREAT | 0666 );
  move = (net_move*)shmat(memdMove, NULL, 0);
  
  memdTurn = shmget(turnKey, sizeof(int), IPC_CREAT | 0666);
  turn = (int*)shmat(memdTurn, NULL, 0);
  
  memdProcNext = shmget(procNextKey, sizeof(int), IPC_CREAT | 0666);
  unsentMoves = (int*)shmat(memdProcNext, NULL, 0);
  //net
  struct sockaddr_in server;
  socklen_t socket_length;
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;
  
  server.sin_port = htons(24601);
 
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  i =  listen( socket_id, 1 );

  char nPBuf[3];
  printf("How many players? (2-5): ");
  while (totalPlayers < 2 || totalPlayers > 5) {
    fgets(nPBuf, 3, stdin);
    if (nPBuf[1] == '\n')
      nPBuf[1] = 0;
    totalPlayers = atoi(nPBuf);
    if (totalPlayers < 2 || totalPlayers > 5) {
      printf("%d is not an acceptable number of players\n", totalPlayers);
      printf("Please pick a number between 2 and 5: ");
    }
  }
  terrs = territories();
  distribute(totalPlayers);
  
  while(1) {

    printf("Awaiting a connection\n");
    socket_length = sizeof(server); 
  
    if (n <= totalPlayers){
      printf("Need %d more players\n", totalPlayers - n + 1);
      socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
      printf("accepted connection %d\n",socket_client);
      i = fork();
      if ( i == 0 ) {
        subserver(socket_client, n);
      } else {
        close(socket_client);
      }
      n++;
      printf("Waiting for new connection\n");
    } else {
      printf("all clients connected\n");
      break;
    }
  } while (*turn > 0) {
    
    sleep(1);
  }
  
  struct shmid_ds d;
  shmctl( memdMove, IPC_RMID, &d );
  
  struct shmid_ds del;
  shmctl( memdTurn, IPC_RMID, &del );
  
  struct shmid_ds delProcNext;
  shmctl( memdProcNext, IPC_RMID, &delProcNext );
  
}
