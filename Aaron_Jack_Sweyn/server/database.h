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

#define NO_CALLBACK 0
#define U_EXISTS_CALLBACK 1
#define GAME_EXISTS_CALLBACK 2
#define GAME_INFO_CALLBACK 3
#define VALIDATE_USER_CALLBACK 4
#define GET_TURN_CALLBACK 5
#define GET_KEY_CALLBACK 6
#define GET_GAMES_IN_PROGRESS_CALLBACK 7
#define U1_TURN 1
#define U2_TURN 2
#define VALID 1647
#define USER_EXISTS 123
#define GAME_EXISTS 124

int createsem; //Semaphore for creating user 
int writesem; //Semaphore for writing
int readsem; //Semaphore for readers
int turnsem;

struct db_game_data_wr * gip_hold;
struct game_info gi;
int db_user_exists(char * name);
int db_my_turn(char *, char *);
int db_validate_user(char *, char *);
//int db_u(char * name, char * opponent); //Returns true or false
int db_get_key(char *, char *);
int db_game_exists(char *, char *);
int db_create_user(char * name, char * password);
void db_create_game(struct game_data *);
void db_update_game(struct game_data *);
struct db_game_data_wr * db_games_in_progress(char * name);
void db_init();
void db_close();
void close_sems();

typedef struct db_game_data{
    int type;
    char u1[50];
    char u2[50];
    int turn;
    int distance;
} db_game_data;


typedef struct db_game_data_wr {
    int number_of_games;
    db_game_data ** games;
} db_game_data_wr;
typedef struct game_info {
    char u1[50];
    char u2[50];
    int u1wins;
    int u2wins;
    int key;
    int dist;
    int turn;
} game_info;
#endif
