/*
 *
 *
 */

#ifndef __DATABASE_AJS
#define __DATABASE_AJS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "server_protocols.h"
#include "compose.h"

#define CREAT_FAILED 0
#define CREAT_SUCC 1

/* Specify the action to be taken for given sql
 * execution callback
 *
 * We specify the request for ease of reading,
 * for example VALIDATE_USER_CALLBACK checks to see
 * if the username/pass  given is valid
 */
#define NO_CALLBACK 0
#define U_EXISTS_CALLBACK 1
#define GAME_EXISTS_CALLBACK 2
#define GAME_INFO_CALLBACK 3
#define VALIDATE_USER_CALLBACK 4
#define GET_TURN_CALLBACK 5
#define GET_KEY_CALLBACK 6
#define GET_GAMES_IN_PROGRESS_CALLBACK 7

/* Turn value stored in database if its
 * u1 or u2's turns... Stored alphabetically remember
 */
#define U1_TURN 0
#define U2_TURN 1
#define NO_WINNER 2 //For when its not U1 or U2

#define VALID 1647 //For true/false returns from
                   //callback function
#define USER_EXISTS 123 //Also for callback return
#define GAME_EXISTS 124
/* To acquire the readsem (which permits infinite readers, but stops
 * writers) we first wait for writers to be clear
 *
 * For the writesem we lock out new readers, wait for current readers
 * to be clear and then we write and release
 */


int createsem; //Semaphore for creating user 
int writesem; //Semaphore for writing
int readsem; //Semaphore for readers

/* GLOBAL LOCKED structs for use in database callbacks
 *
 */
struct db_game_data_wr * gip_hold;
struct db_game_info gi;

/* Username validity
 * For opponent name checking and username taken checks
 */
int db_user_exists(char * name);

/* Returns the turn for two players in a game
 *
 */
int db_my_turn(char *, char *);

/* Confirm proper username and password
 * Everytime we receive data... Maybe a bit
 * excessive. And make it a hash one day
 */
int db_validate_user(char *, char *);

/* Returns the most recent game key
 * between two users
 */
int db_get_key(char *, char *);

/* Check if a game exists between two players
 * 
 */
int db_game_exists(char *, char *);

/* Create a user
 * Checking is done inside of here
 * -Username already taken
 */
int db_create_user(char * name, char * password);

/* Makes a new game; called by update game
 */
void db_create_game(struct cli_upload_game *);

/* Wrapper that both creates new games and updates old
 *
 */
void db_update_game(struct cli_upload_game *, int);

/* Gets games in progress for a user
 * db_game_data_wr stores a list of game data
 */
struct db_game_data_wr * db_games_in_progress(char * name);

/* Init and close functions for cleanup
 *
 */
void db_init();
void db_close();
void close_sems();

// Structs
//

/* Stores game data for CHECK_FOR_GAMES and
 * GAMES_IN_PROGRESS requests
 */
typedef struct db_game_data{
    int type; //Here for easy cast so it can be sent from server to client

    //Our users, alphabetically sorted
    char u1[50];
    char u2[50];

    int turn;
    int distance; //The distance stored in db of the most recent game
    int u1wins;
    int u2wins;
    int last;
} db_game_data;

/* Wrapper struct to hold an list of db_game_data
 *
 */
typedef struct db_game_data_wr {
    int number_of_games;
    db_game_data ** games;
} db_game_data_wr;

/* game_info for STATS 
 * we use a global LOCKED struct gi
 * to store for calls from db_get_game_info
 *
 * Returns more data than just db_game_data
 */
typedef struct db_game_info {
    char u1[50];
    char u2[50];
    int u1wins;
    int u2wins;
    int key;
    int dist;
    int turn;
    int last;
} db_game_info;
#endif
