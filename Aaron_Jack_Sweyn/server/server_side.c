/*
 * Forking server
 * Read requests and respond
 * No real constant connection, except for handshakes
 * Instead we handle reconnections
 *
 *
 */
#include "server_side.h"

int main(int argc, char ** argv) {
    start();
    db_close();
}
void start() {
    //Begins the back end server
    db_init(); //Init database
    listen_for_new_connections();
}
void listen_for_new_connections() {
    global_sock_id = socket( AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( MAIN_PORT );
    int i = bind( global_sock_id, (struct sockaddr *)&server, sizeof(server));
    i = listen( global_sock_id, 0);
    while( 1 ) {
        //Listen for a request
        socklen_t length = sizeof(server);
        //accept response
        int fd = accept(global_sock_id, (struct sockaddr *)&server, &length);
        if( !fork() ){
            handle_connection(fd); 
        }
        close( fd );
    }
}
void handle_connection(int fd) {
    void * in = malloc(400);
    int bytesRead = read(fd, in, 400);
    //Client / server discussion occers in handle_request_type
    int response = handle_request_type((client_out *) in, fd);
    //free(in);
    if( response == SUCC_REQ ) {
        logger("\n\n");
        close(fd);
        exit(0);
    }
}

int handle_request_type(client_out * in, int fd) {

    //Figure out what type of data we are getting and act accordingly
    //Always returns SUCC_REQ... 
    //Client notification and server handling of invalid requests happen here
    if( in->type == CREATE_ACCOUNT ) {
        logger("Registering account...\n");
        cli_creat_acc * request = (cli_creat_acc *) in;
        if( db_create_user( request->name, request->pass)) {
            serv_response * sr = malloc(sizeof(serv_response));
            sr->type = CREATE_ACCOUNT;
            sr->success = 1;
            int s = write( fd, sr, sizeof(serv_response));
            return SUCC_REQ;
        }
        //Username taken...
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = CREATE_ACCOUNT;
        sr->success = 0;
        sr->reason = USERNAME_TAKEN; //Checked by client for why failure
        int s = write( fd, sr, sizeof(serv_response));
        return SUCC_REQ;

    } else if( in->type == REQUEST_TO_PLAY ) {
        printf("Request to play...\n");
        cli_request_game * request = (cli_request_game *) in;    
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = REQUEST_TO_PLAY;
        if( !db_validate_user(request->name, request->pass) ) {
            //Confirm proper password username combo
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else if( !db_user_exists(request->opponent) ) {
            sr->reason = NOT_VALID_OPPONENT;
            sr->success = 0;
        } else if( !db_game_exists(request->name, request->opponent)) {
            //No game between the two so skip to UPLOAD_GAME_RESPONSE in client
            sr->success = 0;
            sr->reason = FIRST_TURN;
        } else if( !db_my_turn(request->name, request->opponent) ) {
            sr->success = 0;
            sr->reason = NOT_MY_TURN;
        } else {
            sr->success = 1;
            sr->key = db_get_key(request->name, request->opponent);
        }
        int w = write( fd, sr, sizeof(serv_response));
        return SUCC_REQ; //Closes socket

    } else if( in->type == UPLOAD_GAME_FIRST || in->type == UPLOAD_GAME_RESPONSE ) {
        printf("Hey\n");
        if( in->type == UPLOAD_GAME_FIRST ) logger("Uploading first game...\n");
        else logger("Uploading second game\n");
        cli_upload_game * request = (cli_upload_game *) in;
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = in->type;
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else {
        sr->success = 1;
        }
        //Second argument will be false if in->type is UPLOAD_GAME_FIRST
        //So then we don't change the turn
        db_update_game(request, in->type - UPLOAD_GAME_FIRST);
        write(fd, sr, sizeof(serv_response));
        return SUCC_REQ;
    } else if ( in->type == CHECK_FOR_GAME ) {
        cli_request_game * request = (cli_request_game *) in;    
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = CHECK_FOR_GAME;
        db_game_data_wr * gd = db_games_in_progress(request->name);
        db_game_data ** gd_proper = calloc(gd->number_of_games, sizeof(db_game_data_wr *));
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else {
            int i = 0;
            sr->success = 1;
            sr->reason = 0;
            while( i < gd->number_of_games ) {
                if( is_my_turn(request->name, gd->games[i]) ) {
                    gd_proper[sr->reason] = gd->games[i];
                    sr->reason++;
                }
                i++;
            }
        }
        int w = write( fd, sr, sizeof(serv_response)); 
        if( sr->success ) {
            int i = 0;
            while(i < sr->reason ) {
                write(fd, gd_proper[i], sizeof(db_game_data));
                i++;
            }
        }
        return SUCC_REQ; //Closes socket

    } else if ( in->type == GAMES_IN_PROG ) {
        //Same process as CHECK_MY_TURN but without is_my_turn trimming
        cli_request_game * request = (cli_request_game *) in;    
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = GAMES_IN_PROG;
        db_game_data_wr * gd = db_games_in_progress(request->name);
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else {
            int i = 0;
            sr->success = 1;
            sr->reason = gd->number_of_games;
        }
        int w = write( fd, sr, sizeof(serv_response));
        if( sr->success ) {
            int i = 0;
            while(i < sr->reason ) {
                write(fd, gd->games[i], sizeof(db_game_data));
                i++;
            }
        }
        return SUCC_REQ; //Closes socket

    } else if ( in->type == GAME_STATS ) {
        cli_creat_acc * request = (cli_creat_acc *) in;
        serv_response * sr = malloc( sizeof(serv_response));
        sr->type = GAME_STATS;
        db_game_data_wr * gd = db_games_in_progress(request->name);
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else {
            int i = 0;
            sr->success = 1;
            sr->reason = gd->number_of_games;
        }
        int w = write( fd, sr, sizeof(serv_response));
        if( sr->success ) {
            int i = 0;
            while(i < sr->reason ) {
                int w = write(fd, gd->games[i], sizeof(db_game_data));
                printf("%s %d %s %d wrote %d\n",gd->games[i]->u1, gd->games[i]->u1wins, gd->games[i]->u2, gd->games[i]->u2wins, w);
                i++;
            }
        }
        return SUCC_REQ; //Closes socket
    }
    return SUCC_REQ;
}
int is_my_turn( char * name, db_game_data * s ) {
    if( strcmp(name, s->u1) == 0) {
        return (s->turn == U1_TURN);
    } else {
        return (s->turn == U2_TURN);
    }
}
void logger(char * str) {
    char * file = "server.log";
    printf("%s", str);
}
