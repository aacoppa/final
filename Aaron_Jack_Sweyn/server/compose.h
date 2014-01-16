/* Compose sql statements
 *
 *
 */

#ifndef __COMPOSE_AJS
#define __COMPOSE_AJS 1

#include "server_protocols.h"
char * initTables();
char * composeUserExists(char *);
char * composeAddUser(char *, int);
char * composeNewGameEntry(struct game_data *);
#endif
