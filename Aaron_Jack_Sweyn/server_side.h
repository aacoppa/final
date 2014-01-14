#ifndef SERVER_SIDE_DEF

#define SERVER_SIDE_AJS 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server_protocols.h"

void start();
void listenForNewConnections();
void acceptNewConnection();
void handleConnectionFault();
void handleRequestType();
#endif
