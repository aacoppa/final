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
    if( global_sock_id == -1 || c == -1) {
        printf("Error connecting to server\n");
        exit(0);
    }
    int s = send_request(type, name, passwd);
    return s;
}
int send_request(int type, char * name, char * passwd) {

    if( type == CREATE_ACCOUNT ) {
        cli_creat_acc * cl = malloc( sizeof(cli_creat_acc) );
        cl->type = type;
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        int s = write( global_sock_id, (char *) cl, sizeof(cli_creat_acc) );
        //Now wait for response
        void * buff = malloc( 400 );
        int bytes_read = read( global_sock_id, buff, 400 );
        serv_response * sr = (serv_response *)buff;
        if( sr->type != CREATE_ACCOUNT ) {
            return CONNECTION_ERROR;
        }
        if( sr->success ) {
            return QUERY_SUCC;
        }
        return sr->reason;
    } else if( type == REQUEST_TO_PLAY ) {
        cli_request_game * cl = malloc( sizeof(cli_request_game) );
        cl->type = type;
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        strcpy(cl->opponent, to_be_sent->opponent);
        int w = write( global_sock_id, cl, sizeof(cli_request_game) );
        //Now wait for response
        void * buff = malloc( 400 );
        int bytes_read = read( global_sock_id, buff, 400);
        serv_response * sr = (serv_response *)buff;
        if( sr->type != REQUEST_TO_PLAY) {
            return CONNECTION_ERROR;
        }
        if( sr->success ) {
            key_to_play = sr->key;
            return QUERY_SUCC;
        }
        return sr->reason;

    } else if( type == UPLOAD_GAME_FIRST || type == UPLOAD_GAME_RESPONSE ) {
       cli_upload_game * cl = malloc( sizeof(cli_upload_game) );
       cl->type = type;
       strcpy(cl->name, name);
       strcpy(cl->pass, passwd);
       strcpy(cl->opponent, to_be_sent->opponent);
       cl->key = to_be_sent->key;
       cl->dist = to_be_sent->dist;
       printf("Key %d Dist %d\n", cl->key, cl->dist);
       void * buff = malloc( 400 );
       write( global_sock_id, cl, sizeof(cli_upload_game) );
       read( global_sock_id, buff, 400);
       serv_response * sr = (serv_response *) buff;
       if( sr->success ) {
            return QUERY_SUCC;
       }
       return sr->reason;

    } else if ( type == CHECK_FOR_GAME ) {
        cli_request_game * cl = malloc( sizeof(cli_request_game) );
        cl->type = type;
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        int w = write( global_sock_id, cl, sizeof(cli_request_game) );
        if( w == -1 ) {
            printf("Error connecting with the server\n");
            exit(0);
        }
        //Now wait for response
        void * buff = malloc( sizeof(serv_response) );

        int bytes_read = read( global_sock_id, buff, sizeof(serv_response));
        serv_response * sr = (serv_response *)buff;
        if( sr->type != CHECK_FOR_GAME) {
            return CONNECTION_ERROR;
        }
        if( !sr->success ) {
            return sr->reason;
        }

        int i = 0;
        db_game_data ** temp_games = calloc(sr->reason + 1, sizeof(db_game_data *));
        while(i < sr->reason) {
            void * b = malloc(sizeof(db_game_data));
            read(global_sock_id, b, sizeof(db_game_data));
            temp_games[i] = malloc(sizeof(db_game_data));
            temp_games[i]->turn = ( (db_game_data *)b)->turn;
            temp_games[i]->last = ((db_game_data *)b)->last; 

            temp_games[i]->distance = ((db_game_data *)b)->turn; 
            strcpy(temp_games[i]->u1, ((db_game_data *)b)->u1); 
            strcpy(temp_games[i]->u1, ((db_game_data *)b)->u2); 
            i++;
        }
        games_returned = temp_games;

        return QUERY_SUCC;

    } else if ( type == GAMES_IN_PROG ) {
        cli_request_game * cl = malloc( sizeof(cli_request_game) );
        cl->type = type;
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        int w = write( global_sock_id, cl, sizeof(cli_request_game) );
        void * buff = malloc(sizeof(serv_response));
        int r= read(global_sock_id, buff, sizeof(serv_response)); 
        serv_response * sr = (serv_response *)buff;
        if( sr->type != GAMES_IN_PROG ) {
            return CONNECTION_ERROR;
        }
        if( !sr->success ) {
            return sr->reason;
        }
        int i = 0;
        db_game_data ** temp_games = calloc(sr->reason + 1, sizeof(db_game_data *));
        while(i < sr->reason) {
            void * b = malloc(sizeof(db_game_data));
            read(global_sock_id, b, sizeof(db_game_data));
            temp_games[i] = malloc(sizeof(db_game_data));
            temp_games[i]->turn = ( (db_game_data *)b)->turn;
            temp_games[i]->last = ((db_game_data *)b)->last; 

            temp_games[i]->distance = ((db_game_data *)b)->distance; 
            strcpy(temp_games[i]->u1, ((db_game_data *)b)->u1); 
            strcpy(temp_games[i]->u2, ((db_game_data *)b)->u2); 
            i++;
        }
        games_returned = temp_games;
        return QUERY_SUCC;
    } else if ( type == GAME_STATS || type == CHECK_FOR_GAME ) {
        cli_creat_acc * cl = malloc( sizeof(cli_creat_acc) );
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        cl->type = type;
        write( global_sock_id, cl, sizeof(cli_creat_acc) );
        void * buff = malloc(sizeof(serv_response));;
        int r = read(global_sock_id, buff, sizeof(serv_response));
        serv_response * sr = (serv_response *)buff;
        if( sr->type != type ) return CONNECTION_ERROR;
        if( !sr->success ) {
            return sr->reason;
        }
        int i = 0;
        db_game_data ** temp_games = calloc(sr->reason + 1, sizeof(db_game_data *));
        while(i < sr->reason) {
            void * b = malloc(sizeof(db_game_data));
            int r = read(global_sock_id, b, sizeof(db_game_data));
            temp_games[i] = malloc(sizeof(db_game_data));
            temp_games[i]->turn = ( (db_game_data *)b)->turn;
            temp_games[i]->distance = ((db_game_data *)b)->distance; 
            temp_games[i]->u1wins = ((db_game_data *)b)->u1wins; 
            temp_games[i]->u2wins = ((db_game_data *)b)->u2wins; 
            temp_games[i]->last = ((db_game_data *)b)->last; 
            strcpy(temp_games[i]->u1, ((db_game_data *)b)->u1); 
            strcpy(temp_games[i]->u2, ((db_game_data *)b)->u2); 
            i++;
        }
        games_returned = temp_games;
        return QUERY_SUCC;
    } else if( type == GET_RANDOM_OPPONENT ) {
        cli_creat_acc * cl = malloc( sizeof(cli_creat_acc) );
        strcpy(cl->name, name);
        strcpy(cl->pass, passwd);
        cl->type = type;
        write( global_sock_id, cl, sizeof(cli_creat_acc) );
        void * buff = malloc(sizeof(serv_response));;
        int r = read(global_sock_id, buff, sizeof(serv_response));
        printf("%d\n", r);
        serv_response * sr = (serv_response *) buff;
        if( sr->reason == NO_UNPLAYED_OPP ) {
            random_opponent = NULL;
        } else {
            printf("HERE\n");
            random_opponent = malloc(50);
            void * b = malloc(50);
            int r = read( global_sock_id, b, 50);
            char * l = b;
            strcpy(random_opponent, l);
            printf("%d %s\n", r, random_opponent);
        }
        return QUERY_SUCC;
    }
    return 0;
}
