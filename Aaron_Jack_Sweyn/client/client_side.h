/* Client Side
 *
 *
 *
 *
 */

#ifndef __CLIENT_SIDE_AJS
#define __CLIENT_SIDE_AJS 1

#include "server_protocols.h"
#include <arpa/inet.h>

#define QUERY_SUCC 0
#define QUERY_FAILED 1
char * IP = "198.211.104.102";
int global_sock_id; //connect
int init_connection();
int send_request();
int listenForResponse();

#endif
