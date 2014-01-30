#include "util.h"

void err(char *m) {
	perror(m);
	exit(EXIT_FAILURE);
}

int saveExists() {
    int makeDir = mkdir("files", 0755);
    // if it's -1, then it failed to create, and the game has already been started
    return (makeDir==-1);
}

void createDevices() {
    
}

