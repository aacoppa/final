#ifndef __SERVER_SIDE_AJS
#define __SERVER_SIDE_AJS 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server_protocols.h"

int global_sock_id; //Accepts new connections
socklen_t sockLength;

void start();
void listenForNewConnections();
void acceptNewConnection();
void handleConnectionFault();
void handleRequestType();
#endif
