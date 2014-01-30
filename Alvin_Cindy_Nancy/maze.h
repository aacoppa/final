#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char* showMaze(const char*,int,int);
void CarveMaze(char*,int,int,int,int);
void generateMaze(char*,int,int);

#endif
