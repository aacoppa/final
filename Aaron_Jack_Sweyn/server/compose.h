/* Compose sql statements
 *
 *
 */

#ifndef __COMPOSE_AJS
#define __COMPOSE_AJS 1

#include "server_protocols.h"
char * composeInitTables();
char * composeGetGameInfo();
char * composeGetGameInfo(char *, char *);
char * composeUpdateWins(char *, char *, int, int);
char * composeAddChallenge(char *, char *, int, int, int);
char * composeGameExists(char *, char *);
char * composeUserExists(char *);
char * composeAddUser(char *, char *);
char * composeGetGamesOf(char *);
char * compose_validate_user(char *, char *);
char * composeNewGameEntry(struct game_data *);
#endif
