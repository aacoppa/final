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

#define CREAT_FAILED 0
#define CREAT_SUCC 1

#define NO_CALLBACK 0
#define U_EXISTS_CALLBACK 1

#define USER_EXISTS 123

int semd; //Semaphore for writing data
int userExists(char * name);
int usersTurn(char * name, char * opponent); //Returns true or false
int createUser(char * name, int passhash);

#endif
