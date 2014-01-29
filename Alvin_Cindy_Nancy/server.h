#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "constants.h"

/*typedef struct roomStruct{
  int timeToStart;
  int roomMembers;
} roomStruct;

typedef struct mazeStruct{
  cell grid[10][10];
  cell startPos;
  cell endPos;
} mazeStruct;
*/

void subserver(int);

int joinRoom(int,int);
int createRoom(int);
int findEmptyRoom();
int leaveRoom(int,int);
int getMaze(int);
int updateScore(int,int,int);
#endif
