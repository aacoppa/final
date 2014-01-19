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

char  * error_messages[10] = {
   "",
   "Username already taken",
   "Invalid username and password combination. Run race login",
   "Sorry, but its not your turn",
   "",
   "No user is logged in. Please run race login or race create",
   "There was an error with the connecting with the server",
   "Sorry but the user you challenged doesn't exist"
};


int key_to_play;
char * IP = "198.211.104.102";
int global_sock_id; //connect
int init_connection();
int send_request();
int listenForResponse();
struct cli_game_data ** games_returned;
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
