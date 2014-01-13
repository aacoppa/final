#ifndef SERVER_SIDE_DEF

#define SERVER_SIDE_DEF 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Define different types of data we can receive
#define CREATE_ACCOUNT 0
#define UPDATE_GAME 1

typedef struct creat_acc {
    int type;
    char name[30];
    char passwd[30];
} creat_acc;

typedef struct game_data {
    int type;
    char from[30];
    char 
    char to[30];
}
#endif
