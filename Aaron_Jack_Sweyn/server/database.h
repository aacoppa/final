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

#define USER_EXISTS 123

int createsem; //Semaphore for creating user 
int writesem; //Semaphore for writing
int readsem; //Semaphore for readers
int userExists(char * name);
int usersTurn(char * name, char * opponent); //Returns true or false
int createUser(char * name, int passhash);
void createGame(struct game_data *);
void updateGame(struct game_data *);
void closeSems();
#endif
