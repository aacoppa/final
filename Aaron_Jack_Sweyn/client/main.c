#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "game.h"
#include "client_side.h"

const char * const error_messages[] = {
   " ",
   "Username already taken",
   "Invalid username and password combination. Run race login",
   "Sorry, but its not your turn",
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

char * user;
char * pass;

void init();
int exec_action(int, char *, char *);
int generate_key();
char * load_input();
void login_user(char *, char *);
char * get_name( char * );
char * get_password( char * );
char * XOR( char, char );
void verify_upass(char *, char *);
void verify_opponent(char *);

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
    else if(strcmp(argv[1], "register") == 0) {
        if( !argv[2] || !argv[3] ) {
            printf("usage: race register [username] [password]\n");
            exit(0);
        }
        verify_upass(argv[2], argv[3]);
        exec_action(CREATE_ACCOUNT, argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "login") == 0) {
        //Change the global login data (~/.rxc/udata)
        if( !argv[2] || !argv[3] ) {
            printf("usage: race login [username] [password]\n");
            exit(0);
        }
        verify_upass(argv[2], argv[3]);
        login_user(argv[2], argv[3]);
        printf("Logged in\n");
        exit(0);

    } else if(strcmp(argv[1], "play") == 0) {
        //challenge a username
        //rxc play [-r] {user}
        //confirms its your turn to play before doing this
        if( !argv[2] ) {
            printf("usage: race play [username]\n");
            exit(0);
        }
        verify_opponent(argv[2]);
        to_be_sent->opponent = malloc(strlen(argv[2]));
        strcpy(to_be_sent->opponent, argv[2]);
        exec_action( REQUEST_TO_PLAY, NULL, NULL );
    } else if(strcmp(argv[1], "pull") == 0) {
        exec_action( CHECK_FOR_GAME, NULL, NULL );
        //Checks if there are any games available for you
    } else if(strcmp(argv[1], "games") == 0) {
        exec_action( GAMES_IN_PROG, NULL, NULL );

    } else if(strcmp(argv[1], "stats") == 0) {
        printf("%s\n", user);
        //Pulls down stats from server to display
        exec_action( GAME_STATS, NULL, NULL );
    } else {
        printf("%s", help_string);
    }
}

void init() {
    games_returned = calloc(sizeof(cli_game_data *), 1);
    srand(time(NULL));
    to_be_sent = malloc( sizeof(request_info) );
    char * input = load_input();
    user = get_name(input);
    pass = get_password(input); 
}
int exec_action(int type, char * name, char * password) {  
    if(!name || !password) {
        name = user; //These two are loaded in init
        password = pass;

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

        if( !games_returned[0] ) {
            printf("No games in progress\n");
        } else {
            printf("Games in progress for %s\n", name);
            printf("USER\tTURN\tYOUR WINS\tTHEIR WINS\nLAST RACE\n");
            int i = 0;
            while(  games_returned[i] ) {
                if(strcmp(name, games_returned[i]->u1) == 0) {

                    printf("%s\t", games_returned[i]->u2);
                    if(games_returned[i]-> turn == U1_TURN) {
                        printf("My Turn\t");
                    } else printf("Their Turn\t");
                    printf("%d\t%d\n", games_returned[i]->u1wins, games_returned[i]->u2wins);
                    
                } else {
                    printf("%s\t", games_returned[i]->u1);
                    if(games_returned[i]-> turn == U2_TURN) {
                        printf("My Turn");
                    } else printf("Their Turn");
                    printf("%d\t%d\n", games_returned[i]->u2wins, games_returned[i]->u1wins);
                }
                i++;
            }
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
            printf("Key and dist set\n");
            return exec_action(UPLOAD_GAME_RESPONSE, name, password);
    }
    if(type == UPLOAD_GAME_RESPONSE) {
        
        return 0;
    }
    
    if(type == GAME_STATS) {
        if( !games_returned[0] ) {
            printf("No games in progress\n");
        } else printf("You\tWins\tThem\tWins\tLast Winner\tTurn\n");
        int i = 0;
        while(  games_returned[i] ) {
            char * last_str = malloc(40);
            if(strcmp(name, games_returned[i]->u1) == 0) { 
                if(games_returned[i]->last == NO_WINNER) strcpy(last_str, "Waiting");
                else if(games_returned[i]->last == U1_TURN) strcpy(last_str, "You Won");
                else strcpy(last_str, "They won");
                printf("%s\t%d\t%s\t%d\t%s", games_returned[i]->u1, games_returned[i]->u1wins, games_returned[i]->u2, games_returned[i]->u2wins, last_str);
            } else {
                if(games_returned[i]->last == NO_WINNER) strcpy(last_str, "Waiting");
                else if(games_returned[i]->last == U1_TURN) strcpy(last_str, "They Won");
                else strcpy(last_str, "You won");
                printf("%s\t%d\t%s\t%d\t%s", games_returned[i]->u2, games_returned[i]->u2wins, games_returned[i]->u1, games_returned[i]->u1wins, last_str);
            }
            i++;
        }

    }
    return 0;
}   
/* Store this data in a file
*/

char * load_input() {
    struct passwd *pw = getpwuid(getuid());
    char * u_file= malloc(400);
    strcpy(u_file, pw->pw_dir);
    strcat(u_file, "/.rxc/udata");
    int fd = open(u_file, O_RDONLY);
    char * input = malloc(400);
    read(fd, input, 400);
    close( fd );
    return input;
}
char * XOR(char val, char key) {
    char * a = (char *) malloc(sizeof(char));
    *a = val ^ key;
    return a;
}
char * get_output(char * name, char * password) {
    char * output = malloc(101);
    int i = 0;
    while(name[i]) {
        output[i] = *( XOR(name[i], 'z') );
        i++;
    }
    output[i] = '\0';
    int j = 1;
    while(password[j-1]) {
        output[i+j] = *( XOR(password[j-1], 'z'));
        j++;
    }
    return output;
}
char * get_name( char * input ) {
    if( input == NULL ) return NULL;
    char * name = malloc(strlen(input) + 1);
    int i = 0;
    while(input[i] != '\0') {
        name[i] = *( XOR(input[i], 'z') );
        i++;
    }
    return name;
}
char * get_password( char * input ) {
    if( input == NULL ) return NULL;
    char * password = malloc(strlen(input) + 1);
    int i = 0;
    while(input[i] != '\0') {
        i++;
    }
    int j = 0;
    i++;
    while( input[i] ) {
        password[j] = *( XOR(input[i], 'z') );
        j++;
        i++;
    }
    return password;
}
void login_user(char * name, char * password) {
    struct passwd *pw = getpwuid(getuid());
    char * u_file= malloc(400);
    strcpy(u_file, pw->pw_dir);
    strcat(u_file, "/.rxc/udata");
    int fd = open(u_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
    char * output = get_output(name, password);
    int s = write(fd, output, 101); 
    close( fd );
}
int generate_key() {
    return rand();
}

void verify_opponent(char * name) {
    if(strlen(name) >= 50 || strchr(name, '\"') ) {
        printf("The maximum name length is 50 characters\n\
                Also you can't use quotes ( \" ) in your submission\n\
                Please try again.\n");
        exit(0);
    }
}
void verify_upass(char * name, char * pass) {
    if(strlen(name) >= 50 || strlen(pass) >= 50 || strchr(name, '\"') || strchr(pass, '\"')) {
        printf("Sorry that username password combination is invalid\n\
                The maximum name and password length is 50 charecters each\n\
                Also you can't use quotes ( \" ) in your submission\n\
                Please try again.\n");
        exit(0);
    }
}

