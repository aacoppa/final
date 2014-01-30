#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

#define loop for(;;)
#define PRINCE_NAME "Jamal Kareem Tijani"

// Prints an error message
void err(char *m);
void printColor(char *s, char *color);

int dirExists();
char *currentDir(); // gets current folder (not entire directory)
void createFile(char *name);

int saveExists();
void createDevices(); // creates all the folders and shit

void goToRoot(); // goes to root files/
void goToRandomInner(); // goes to a random innermost folder

void hidePrince(); // when the prince hides in a random folder and you gotta find him
