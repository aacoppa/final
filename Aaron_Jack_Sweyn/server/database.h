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
#define U1_TURN 1
#define U2_TURN 2
#define VALID 1647
#define USER_EXISTS 123
#define GAME_EXISTS 124

int createsem; //Semaphore for creating user 
int writesem; //Semaphore for writing
int readsem; //Semaphore for readers
int turnsem;
struct game_info gi;
struct game_info turn_info;

int userExists(char * name);
int usersTurn(char * name, char * opponent); //Returns true or false
int gameExists(char *, char *);
int createUser(char * name, char * password);
void createGame(struct game_data *);
void updateGame(struct game_data *);

void initDB();
void closeSems();

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
