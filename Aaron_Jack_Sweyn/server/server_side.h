#ifndef __SERVER_SIDE_AJS
#define __SERVER_SIDE_AJS 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server_protocols.h"

#define SUCC_REQ 1
#define INVALID_USER 2 
#define NOT_MY_TURN 3

int global_sock_id; //Accepts new connections

void start();
void listenForNewConnections();
void handleConnection();
void acceptNewConnection();
void handleConnectionFault();
int handleRequestType();
int hashPassword(char *);
#endif
