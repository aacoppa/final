/* Server protocols and constants
 * To be used by client/server
 *
 *
 *
 */
#ifndef __SERV_MESG_ASJ
#define __SERV_MESG_ASJ 1

//include all of our needed headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Define different types of data we can send/receive
#define CREATE_ACCOUNT 0
#define UPLOAD_GAME 1
#define CHECK_FOR_GAME 2
#define GAMES_IN_PROG 3
#define GAME_STATS 4
#define LOGIN 5
#define REQUEST_TO_PLAY 6
//Reasons...
#define USERNAME_TAKEN 1
#define INVALID_UPASS 2
#define NOT_MY_TURN 3

//Data server receives / Client sends out
typedef struct serv_response {
    int type;
    int success;
    int reason;
    int key;
} serv_response;
typedef struct client_out {
    int type;
} client_out;

typedef struct game_data {
    char from[50];
    char to[50];
    int u1wins;
    int genkey;
    int dist; //For response to the challenge
    int nextdist; //For the second recording...
} game_data;

typedef struct cli_creat_acc {
    int type;
    char name[50];
    char pass[50];
} cli_creat_acc;

typedef struct cli_upload_game {
    int type;
    char name[50];
    char pass[50];
    char opponent[50];
    int first_dist;
    int second_dist;
    int key;
} cli_upload_game;

typedef struct cli_request_game {
    int type;
    char name[50];
    char pass[50];
    char opponent[50];
} cli_request_game;
#endif
