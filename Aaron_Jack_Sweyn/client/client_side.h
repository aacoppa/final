/* Client Side
 *
 *
 *
 *
 */

#ifndef __CLIENT_SIDE_AJS
#define __CLIENT_SIDE_AJS 1

#include "server_protocols.h"
#include <arpa/inet.h>

#define QUERY_SUCC 0
#define QUERY_FAILED 1

int key_to_play;
char * IP = "198.211.104.102";
int global_sock_id; //connect
int init_connection();
int send_request();
int listenForResponse();
char error_messages = {
   "",
   "Username already taken",
   "Invalid username and password combination",
   "Sorry, but its not your turn"
}

typedef struct cli_game_data {
    int type;
    char u1[50];
    char u2[50];
    int turn;
    int distance;
} cli_game_data;

typedef struct request_info {
    char * name;
    char * passwd;
    char * opponent;
    int first_dist;
    int second_dist;
}
#endif
