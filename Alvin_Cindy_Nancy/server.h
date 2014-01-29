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

int** rooms;
typedef struct roomStruct{
  int timeToStart;
  int roomMembers;
} roomStruct;

typedef struct mazeStruct{
  cell grid[10][10];
  cell startPos;
  cell endPos;
} mazeStruct;

int findEmptyRoom();
void createRoom(int);

void subserver(int);
#endif
