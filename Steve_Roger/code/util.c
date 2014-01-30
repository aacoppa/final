#include "util.h"

void err(char *m) {
	perror(m);
	exit(EXIT_FAILURE);
}

int dirExists(char *dir) {
    struct stat s;
    int err = stat(dir, &s);
    return (err != -1);
}

int saveExists() {
    int makeDir = mkdir("files", 0755);
    // if it's -1, then it failed to create, and the game has already been started
    return (makeDir==-1);
}
void createDevices() {
    chdir("files");
    char f[48][25] = {"Computer","down","Documents","down","Work","School","Not Porn","up","Games","down","Club Penguin","League of Legends","Runescape","up","System","down","Programs","Data","up","up",
                      "Phone","down","Documents","down","Music","Pictures","Videos","up","System","down","Programs","Data","up","up",
                      "Tablet","down","Documents","down","Music","Books","Movies","up","System","down","Programs","Data","up","up"};
    int i = -1;
    char last[25];
    while(++i < (sizeof(f)/sizeof(f[0]))) {
        if (strcmp(f[i],"down")==0) {
            chdir(last);
        } else if (strcmp(f[i],"up")==0) {
            chdir("..");
        } else {
            mkdir(f[i], 0755);
            strcpy(last, f[i]);
        }
    }
    chdir("Computer");
    goToRoot();
    mkdir("penis", 0755);
    goToRandomInner();
}

void goToRoot() {
    char cwd[1024];
    int i=0;
    while (++i<50) {
        getcwd(cwd, sizeof(cwd));
    }
}
void goToRandomInner() {
    
}





