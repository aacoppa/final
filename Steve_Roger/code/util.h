#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define loop for(;;)

// Prints an error message
void err(char *m);

int saveExists();
void createDevices();
