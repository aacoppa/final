#ifndef __SERVER_SIDE_AJS
#define __SERVER_SIDE_AJS 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server_protocols.h"
#include "database.h"

#define SUCC_REQ 1 //Returned from handle_request_type

int global_sock_id; //Accepts new connections


//Structs
//

/* Used in first read call, later cast to different
 * struct * depending upon type
 */
typedef struct client_out {
    int type;
} client_out;


/* Init database, etc...
 *
 */
void start();

/* Constantly fork new connections
 *
 */
void listen_for_new_connections();

/* First read in here, than pass on to type handling
 *
 */
void handle_connection();

/* Function that deals with client/server discussion
 */
int handle_request_type();

/* Called to confirm that a play request is valid
 *
 */
int is_my_turn(char * name, db_game_data *);

/* Log function
 *
 */
void logger(char *);
#endif
