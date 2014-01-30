#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <signal.h>
#include "constants.h"
#include "maze.h" 

void subserver(int);
char* getMaze(char[256],int,int);
int updateScore(char*);
#endif
