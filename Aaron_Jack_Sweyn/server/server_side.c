/*
 * Forking server
 * Read requests and respond
 * No real constant connection, except for handshakes
 * Instead we handle reconnections
 *
 *
 */
#include "server_side.h"
#include "database.h"

int main(int argc, char ** argv) {
    db_init();
    cli_creat_acc * i = malloc(sizeof(client_out) );
    i->type = CREATE_ACCOUNT;
    strcpy(i->name, "aaron");
    strcpy(i->pass, "tol");
    handle_request_type(i, 200);
    handle_request_type(i, 200);
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
    }
}
void handle_connection(int fd) {
    while( 1 ) {
        void * in = malloc(400);
        int bytesRead = read(fd, in, sizeof(in));
        int response = handle_request_type((client_out *) in);
        free(in);
        if( response == SUCC_REQ ) {
            close(fd);
            exit(0);
        }
    }
}
int handle_request_type(client_out * in, int fd) {

    if( in->type == CREATE_ACCOUNT ) {
        cli_creat_acc * request = (cli_creat_acc *) in;
        //int passHash = hashPassword(request->pass);
        if( db_create_user( request->name, request->pass)) {
            serv_response * sr = malloc(sizeof(serv_response));
            sr->type = CREATE_ACCOUNT;
            sr->success = 1;
            //write( fd, sr, sizeof(sr) );
            printf("Success\n");
            return SUCC_REQ;
        }
        //Failure
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = CREATE_ACCOUNT;
        sr->success = 0;
        sr->reason = USERNAME_TAKEN;
        //write( fd, sr, sizeof(sr) );
        printf("Failure: name taken");
        return SUCC_REQ;

    } else if( in->type == REQUEST_TO_PLAY ) {
        cli_request_game * request = (cli_request_game *) in;    
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = REQUEST_TO_PLAY;
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else if( !db_game_exists(request->name, request->opponent)) {
            sr->success = 0;
            sr->reason = FIRST_TURN;
        } else if( !db_user_exists(request->opponent) ) {
            sr->reason = NOT_VALID_OPPONENT;
            sr->success = 0;
        } else if( !db_my_turn(request->name, request->opponent) ) {
            sr->success = 0;
            sr->reason = NOT_MY_TURN;
        } else {
            sr->success = 1;
            sr->key = db_get_key(request->name, request->opponent);
        }
        write( fd, sr, sizeof(sr));
        return SUCC_REQ; //Closes socket

    } else if( in->type == UPLOAD_GAME ) {
        cli_upload_game * request = (cli_upload_game *) in;
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = UPLOAD_GAME;
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        }
        sr->success = 1;
        db_update_game((game_data *) in);
        write(fd, sr, sizeof(serv_response));
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
            while( i <= gd->number_of_games ) {
                if( is_my_turn(request->name, (serv_out_games *) gd->games[i]) ) {
                    gd_proper[sr->reason] = gd->games[i];
                    sr->reason++;
                }
                i++;
            } 
        }
        write( fd, sr, sizeof(sr));
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
        db_game_data ** gd_proper = calloc(gd->number_of_games, sizeof(db_game_data_wr *));
        if( !db_validate_user(request->name, request->pass) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS;
        } else {
            int i = 0;
            sr->success = 1;
            sr->reason = gd->number_of_games;
        }
        write( fd, sr, sizeof(sr));
        if( sr->success ) {
            int i = 0;
            while(i < sr->reason ) {
                write(fd, gd_proper[i], sizeof(db_game_data));
                i++;
            }
        }
        return SUCC_REQ; //Closes socket

    } else if ( in->type == GAME_STATS ) {
    
    }
    return 0;
}
int hash_password(char * passwd) {
    int ret;
    return ret;
}
int is_my_turn( char * name, serv_out_games * s ) {
    if( strcmp(name, s->u1) == 0) {
        return (s->turn == U1_TURN);
    } else {
        return (s->turn == U2_TURN);
    }
}
