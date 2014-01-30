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
#define UPLOAD_GAME_FIRST 1
#define CHECK_FOR_GAME 2
#define GAMES_IN_PROG 3
#define GAME_STATS 4
#define LOGIN 5
#define REQUEST_TO_PLAY 6
#define UPLOAD_GAME_RESPONSE 7

//Reasons for failure...
#define USERNAME_TAKEN 1
#define INVALID_UPASS 2
#define NOT_MY_TURN 3
#define FIRST_TURN 4
#define NOT_LOGGED_IN 5
#define CONNECTION_ERROR 6
#define NOT_VALID_OPPONENT 7

#define MAIN_PORT 35412

//Structs
//

/* Typical response to be sent to client for 
 * play, login, register requests
 *
 * Client also uses this struct
 */
typedef struct serv_response {
    int type; //If type is game_data then it'll wait
    int success;
    int reason;
    int key;
} serv_response;


/* Reponse to games, pull request
 *
 */
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
    int dist;
    int key;
} cli_upload_game;

typedef struct cli_request_game {
    int type;
    char name[50];
    char pass[50];
    char opponent[50];
} cli_request_game;

typedef struct stat_data {
    int type;
    char u1[50];
    char u2[50];
    int u1wins;
    int u2wins;
    int turn;
} stat_data;
#endif
