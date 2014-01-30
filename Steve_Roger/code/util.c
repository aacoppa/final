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
char *currentDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    int len = strlen(cwd);
    char *wd = cwd;
    wd += len - 1;
    while (*(--wd) != '/') ;
    wd++;
    return wd;
}
void createFile(char *name) {
    int fd = open(name, O_WRONLY | O_cREAT);
    close(fd);
}

int saveExists() {
    int makeDir = mkdir("files", 0755);
    chdir("files"); // go into files
    return (makeDir==-1); // if it's -1, then it failed to create, and the game has already been started
}
void createDevices() {
    // giant array of all the stuff. down/up moves along the directory so I don't need wack as fuck multidimensional arrays
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
}

void goToRoot() {
    while (strcmp(currentDir(),"files")!=0) chdir("..");
}
void goToRandomInner() {
    goToRoot();
    int i;
    for(i=0;i<2;i++) { // do it twice because all innermost directories are 2 levels deep
        /* This is the most ghetto way to pick a random subdirectory
         * Get the # of directories, x
         * Then loop through the directories, probability of choosing the first one is 1/x
         * If not, the second one has a 1/(x-1) chance, and then 1/(x-2) and so on until you reach 1/1 and that's the last directory
         */
        DIR *dp;
        struct dirent *ep;
        dp = opendir("./");
        double len = 0;
        while ((ep = readdir(dp))) {
            if (strcmp(ep->d_name,".")==0 || strcmp(ep->d_name,"..")==0) continue;
            len++;
        }
        closedir(dp);
        dp = opendir("./");
        while ((ep = readdir(dp))) {
            if (strcmp(ep->d_name,".")==0 || strcmp(ep->d_name,"..")==0) continue;
            if ((rand() / (double)RAND_MAX) <= (1.0 / len)) {
                chdir(ep->d_name);
                break;
            }
            len--;
        }
        closedir(dp);
    }
}





