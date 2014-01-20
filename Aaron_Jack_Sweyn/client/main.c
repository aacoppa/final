#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "game.h"
#include "client_side.h"

const char * const error_messages[] = {
   " ",
   "Username already taken",
   "Invalid username and password combination. Run race login",
   "Sorry, but its not your turn"
   " ",
   "No user is logged in. Please run race login or race create",
   "There was an error with the connecting with the server",
   "Sorry but the user you challenged doesn't exist"
};

const char * help_string = "rxc help\n\
login   \t [username] [password]\t Log into this computer\n\
register\t [username] [password]\t Register a username\n\
play    \t [opponent]           \t Challenge an opponent\n\
pull    \t                      \t Checks for challenges\n\
games   \t                      \t Displays games in progress\n\
stats   \t                      \t Displays statistics\n\
help    \t                      \t Print this help message\n";



void init();
int exec_action(int, char *, char *);
int generate_key();
char * load_user_name();
char * load_password();
char * login_user(char *, char *);

int main(int argc, char ** argv) {
    init();
    //Parse arguments for what to do
    if(argc < 2) {
        //Print help message...
        printf("%s", help_string);
        exit(0);
    }
    if(strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("%s", help_string);
        exit(0);
    }
    if(strcmp(argv[1], "register") == 0) {
        if( !argv[2] || !argv[3] ) {
            printf("usage: race register [username] [password]\n");
            exit(0);
        }
        exec_action(CREATE_ACCOUNT, argv[2], argv[3]);
    }
    if(strcmp(argv[1], "login") == 0) {
        //Change the global login data (~/.rxc/udata)
        if( !argv[2] || !argv[3] ) {
            printf("usage: race login [username] [password]\n");
            exit(0);
        }
        login_user(argv[2], argv[3]);
        exit(0);

    } else if(strcmp(argv[1], "play") == 0) {
        //challenge a username
        //rxc play [-r] {user}
        //confirms its your turn to play before doing this
        if( !argv[2] ) {
            printf("usage: race play [username]\n");
            exit(0);
        }
        strcpy(to_be_sent->opponent, argv[2]);
        exec_action( REQUEST_TO_PLAY, NULL, NULL );
    } else if(strcmp(argv[1], "pull") == 0) {
        exec_action( CHECK_FOR_GAME, NULL, NULL );
        //Checks if there are any games available for you
    } else if(strcmp(argv[1], "games") == 0) {
        exec_action( GAMES_IN_PROG, NULL, NULL );

    } else if(strcmp(argv[1], "stats") == 0) {
        //Pulls down stats from server to display
    } 
}

void init() {
    srand(time(NULL));
    to_be_sent = malloc( sizeof(request_info) );
}
int exec_action(int type, char * name, char * password) {  
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
                
                printf("%s\t", games_returned[i]->u2);
                if(games_returned[i]-> turn == U1_TURN) {
                    printf("My Turn\n");
                } else printf("Their Turn\n");
            } else {
                printf("%s\t", games_returned[i]->u1);
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
                printf("%s\t", games_returned[i]->u2);
            } else {
                printf("%s\t", games_returned[i]->u1);
            }
            i++;
        }
    } else if(type == REQUEST_TO_PLAY) {
        int dist = game_start(key_to_play);
        to_be_sent->dist = dist;
        to_be_sent->key = key_to_play;
        return exec_action(UPLOAD_GAME_FIRST, name, password);
    } 
    if(type == UPLOAD_GAME_FIRST) {
        first_game:
            to_be_sent->key = generate_key();
            to_be_sent->dist = game_start(to_be_sent->key);
            return exec_action(UPLOAD_GAME_RESPONSE, name, password);
    }
    if(type == UPLOAD_GAME_RESPONSE) {
        return 0;
    }
    return 0;
}   
/* Store this data in a file
*/
char * login_user(char * name, char * password) {

    return NULL;
}
char * load_user_name() {

    return NULL;
}

char * load_password() {

    return NULL;
}
int generate_key() {
    return rand();
}
