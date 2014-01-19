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
    if(argv[1] == "register") {
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

int send_request(int type, char * name, char * password) {  
    if(!name || !password) {
        name = load_user_name();
        password = load_password();
        if( !name || !password) {
            printf("%s\n", error_messages[NOT_LOGGED_IN]);
            return 0;
        }
    }
    int ret = init_connection(type, name, password);
    if( ret == FIRST_TURN ) {
        goto first_game;
    }
    if( ret ) {
        printf("%s\n", error_messages[ret]);
        return 0;
    }
    /* Successful requests we must do something
     *
     */

    if( type == CREATE_ACCOUNT ) {
        login_user(name, password);
        printf("Created account and logged in\n");
        return 1;


    } else if( type == GAMES_IN_PROG) {
        printf("Games in progress for %s\n", name);
        printf("USER\tTURN\n");
        int i = 0;
        while(  games_returned[i] ) {
            if(strcmp(name, games_returned[i]->u1) == 0) {
                
                printf("%s\t", games_returned->u2);
                if(games_returned[i]-> turn == U1_TURN) {
                    printf("My Turn\n");
                } else printf("Their Turn\n");
            } else {
                printf("%s\t", games_returned->u1);
                if(games_returned[i]-> turn == U2_TURN) {
                    printf("My Turn\n");
                } else printf("Their Turn\n");
            }
            i++;
        }
    } else if(type == CHECK_FOR_GAME) {
        if( !games_returned[0] ) {
            printf("No games returned\n");
        } else printf("My turn against:\n");
        int i = 0;
        while(  games_returned[i] ) {
            if(strcmp(name, games_returned[i]->u1) == 0) { 
                printf("%s\t", games_returned->u2);
            } else {
                printf("%s\t", games_returned->u1);
            }
            i++;
        }
    } else if(type == REQUEST_TO_PLAY) {
        int dist = game_start(key_to_play);
        //upload the first game
    } 
    
    if(type == UPLOAD_GAME) {
        first_game:
            key_to_play = generate_key();     
            int dist = game_start(key);
    }
}   
/* Store this data in a file
*/
char * login_user(char * name, char * password) {

}
char * load_user_name() {

}

char * load_password() {

}
void startGame() {
    t1 = clock();
    runGame(mapKey); //Runs the game in game.c
    t2 = clock();

    time = (t2 - t1) / CLOCKS_PER_SEC;
}

