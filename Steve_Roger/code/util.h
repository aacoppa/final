#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define loop for(;;)

// Prints an error message
void err(char *m);

int dirExists();
char *currentDir(); // gets current folder (not entire directory)
void createFile(char *name);

int saveExists();
void createDevices(); // creates all the folders and shit

void goToRoot(); // goes to root files/
void goToRandomInner(); // goes to a random innermost folder
