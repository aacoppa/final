#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv) {
    //Parse arguments for what to do
    if(argc < 2) {
        //Print help message...

        exit(0);
    }
    if(argv[1] == "login") {
        //Change the global login data (~/.rxc/udata)
    } else if(argv[1] == "play") {
        //challenge a username
        //rxc play [-r] {user}
        //confirms its your turn to play before doing this
    } else if(argv[1] == "pull") {
        //Checks if there are any games available for you
    } else if(argv[1] == "stats") {
        //Pulls down stats from server to display
    } else if(argv[1] == "mkuser") {
        //Create a username and password
    } 
}

void playGame() {

}

