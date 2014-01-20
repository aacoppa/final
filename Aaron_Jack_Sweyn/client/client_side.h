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

#define U1_TURN 1
#define U2_TURN 2

#define IP "198.211.104.102"

int key_to_play;
int global_sock_id; //connect
int init_connection();
int send_request();
struct cli_game_data ** games_returned;
struct request_info * to_be_sent;
typedef struct db_game_data{
    int type; //Here for easy cast so it can be sent from server to client

    //Our users, alphabetically sorted
    char u1[50];
    char u2[50];

    int turn;
    int distance; //The distance stored in db of the most recent game
} db_game_data;


typedef struct cli_game_data {
    int type;
    char u1[50];
    char u2[50];
    int turn;
    int dist;
} cli_game_data;

typedef struct request_info {
    char * name;
    char * passwd;
    char * opponent;
    int dist;
    int key;
} request_info;
#endif
