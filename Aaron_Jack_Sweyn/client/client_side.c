#include "client_side.h"

int init_connection(int type, char * name, char * passwd) {
    cli_creat_acc * cr = malloc( sizeof(cli_creat_acc) );
    cr->type = CREATE_ACCOUNT;
    struct sockaddr_in sock;
    global_sock_id = socket( AF_INET, SOCK_STREAM, 0 );
    sock.sin_family = AF_INET;
    inet_aton( IP, &(sock.sin_addr) );
    sock.sin_port = htons(MAIN_PORT);
    int c = connect( global_sock_id, (struct sockaddr *)&sock, sizeof(sock) );
    int s = send_request(type, name, passwd);
}
int send_request(int type, request_info rq) {
    if( type == CREATE_ACCOUNT ) {
        cli_creat_acc * cl = malloc( sizeof(cli_creat_acc) );
        cl->type = type;
        strcpy(cl->name, rq->name);
        strcpy(cl->passwd, rq->passwd);
        write( global_sock_id, *cl, sizeof(cl) );
        //Now wait for response
        void * buff = malloc( 400 );
        int bytes_read = read( global_sock_id, buff, sizeof(buff) );
        serv_response sr = (serv_response *)&buff;
        if( sr->type != CREATE_ACCOUNT ) {
            return QUERY_FAILED;
        }
        if( sr->success ) {
            return QUERY_SUCC;
        }
        return sr->reason;
    } else if( type == REQUEST_TO_PLAY ) {
        cli_request_game * cl = malloc( sizeof(cli_request_game) );
        cl->type = type;
        strcpy(cl->name, rq->name);
        strcpy(cl->passwd, rq->passwd);
        strcpy(cl->opponent, rq->opponent);
        write( global_sock_id, *cl, sizeof(cl) );
        //Now wait for response
        void * buff = malloc( 400 );
        int bytes_read = read( global_sock_id, buff, sizeof(buff) );
        serv_response sr = (serv_response *)&buff;
        if( sr->type != REQUEST_TO_PLAY) {
            return QUERY_FAILED;
        }
        if( sr->success ) {
            key_to_play = sr->key;
            return QUERY_SUCC;
        }
        return sr->reason;

    } else if( type == UPLOAD_GAME ) {

    } else if ( type == CHECK_FOR_GAME ) {
        cli_request_game * cl = malloc( sizeof(cli_request_game) );
        cl->type = type;
        strcpy(cl->name, rq->name);
        strcpy(cl->passwd, rq->passwd);
        write( global_sock_id, cl, sizeof(cli_request_game) );
        //Now wait for response
        void * buff = malloc( 400 );
        int bytes_read = read( global_sock_id, buff, sizeof(buff) );
        serv_response sr = (serv_response *)&buff;
        if( sr->type != CHECK_FOR_GAME) {
            return QUERY_FAILED;
        }
        if( sr->success ) {
            int i = 0;
            cli_game_data ** cd = calloc(sr->reason, sizeof(cli_game_data *));
            while( i < sr->reason ) {
                read(fd, cd[i], sizeof(cli_game_data));
            }
        }
        return sr->reason;

    } else if ( type == GAMES_IN_PROG ) {

    } else if ( type == GAME_STATS ) {

    } else if ( type == LOGIN ) {

    }
    return 0;

}
