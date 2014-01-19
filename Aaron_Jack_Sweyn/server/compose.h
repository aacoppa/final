/* Compose sql statements
 * nothing is executed here
 * instead strings are composed and returned to be run
 *
 */

#ifndef __COMPOSE_AJS
#define __COMPOSE_AJS 1

#include "server_protocols.h" //Needed for struct game_data
/*Initializes tables
 * game_data stores data about game
 * users stores username password pair
 */
char * compose_init_tables();

/* Returns information about game between two players
 * Depending upon callback function will give different information
 */

char * compose_get_game_info(char *, char *);

/* Updates the number of wins for each user
 *
 */
char * compose_update_wins(char *, char *, int, int);

/* Replaces old game entry with newer data:\
 * turn, dist, key
 *
 */
char * compose_add_challenge(char *, char *, int, int, int);

/* Checks if a game exists
 *
 */
char * compose_game_exists(char *, char *);

/* Checks if user exists
 *
 */
char * compose_user_exists(char *);

/* Adds a user to the database
 *
 */
char * compose_add_user(char *, char *);

/* Returns all the games of a user
 *
 */
char * compose_get_games_of(char *);

/* Used to confirm that a user/pass combo is valid
 *
 */
char * compose_validate_user(char *, char *);

/* Creates a new game between two users
 *
 */
char * compose_new_game_entry(struct game_data *);
#endif
