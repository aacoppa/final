/* Server protocols and constants
 * To be used by client/server
 *
 *
 *
 */
#ifndef SERV_MESG_ASJ
#define SERV_MESG_ASJ 1

//include all of our needed headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Define different types of data we can receive
#define CREATE_ACCOUNT 0
#define CREATE_GAME 1
#define UPLOAD_GAME 2
#define CHECK_FOR_GAME 3


//Data server receives / Client sends out

typedef struct game_data {
    char from[50];
    char to[50];
    u_int32_t genkey;
    int time;
} game_data;
typedef struct cli_out_creat_acc {
    int type;
    char name[30];
    char passwd[30];
} creat_acc;

typedef struct cli_out_game_data {
    int type;
    char from[30];
    char to[30];
} cli_out_game_data;

//Data server sends out / Client receives
typedef struct serv_out_creat_acc {
    int type;
    char name[30];
    char passwd[30];
    int success; //Whether or not we created correctly
} serv_out_creat_acc;

#endif
