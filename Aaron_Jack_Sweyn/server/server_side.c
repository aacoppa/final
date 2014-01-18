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
    start();
}
void start() {
    //Begins the back end server
    initDB(); //Init database
    listenForNewConnections();
}
void listenForNewConnections() {
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
            handleConnection(fd); 
        }
    }
}
void handleConnection(int fd) {
    while( 1 ) {
        void * in = malloc(400);
        int bytesRead = read(fd, in, sizeof(in));
        int response = handleRequestType((client_out *) in);
        free(in);
        if( response == SUCC_REQ ) {
            close(fd);
            exit(0);
        }
    }
}
int handleRequestType(client_out * in, int fd) {

    if( in->type == CREATE_ACCOUNT ) {
        cli_creat_acc * request = (cli_creat_acc *) in;
        int passHash = hashPassword(request->pass);
        if( createUser( request->name, passHash)) {
            serv_response * sr = malloc(sizeof(serv_response));
            sr->type = CREATE_ACCOUNT;
            sr->success = 1;
            write( fd, sr, sizeof(sr) );
            return SUCC_REQ;
        }
        //Failure
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = CREATE_ACCOUNT;
        sr->success = 0;
        sr->reason = USERNAME_TAKEN;
        write( fd, sr, sizeof(sr) );
        return SUCC_REQ;

    } else if( in->type == REQUEST_TO_PLAY ) {
        cli_request_game * request = (cli_request_game *) in;
        
        serv_response * sr = malloc(sizeof(serv_response));
        sr->type = REQUEST_TO_PLAY;
        if( !validate_user(request->name, request->password) ) {
            sr->success = 0;
            sr->reason = INVALID_UPASS
        } else if( !db_my_turn(request->name, request->opponent) ) {
            sr->success = 0;
            sr->reason = NOT_MY_TURN;
        } else {
            sr->success = 1;
            sr->key = db_get_key(request->name, request->opponent);
        }
        write( fd, sr, sizeof(sr));
        returrn SUCC_REQ; //Closes socket
    } else if( in->type == UPLOAD_GAME ) {
        cli_upload_game * request = (cli_upload_game *) in;

    } else if ( in->type == CHECK_FOR_GAME ) {

    } else if ( in->type == GAMES_IN_PROG ) {

    } else if ( in->type == GAME_STATS ) {

    } else if ( in->type == LOGIN ) {

    }
    return 0;
}
void handleConnectionFault() {

}
int hashPassword(char * passwd) {
    int ret;
    
}
