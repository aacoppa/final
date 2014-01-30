#ifndef NET_NETINIT_H
#define NET_NETINIT_H
// Files to initialize this computer into the network, basically get a list of interpreters to talk to
// It also is expected to set up a listen for incoming requests and respond to them with a list of servers and
// to add the connecting server to the list of other servers this one knows about.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "parser/types.h"

// Header specifying a request for a list of servers
#define REQUEST_LIST "REQLIST"
// Header specifying that the server want's us to know it exists
#define REQUEST_INFORM "REQINFORM"
#define INFORM_DEAD "DEAD"
#define INFORM_LIVE "LIVE"
// Header specifying a request for a function to get evaluated
#define REQUEST_FN "REQFN"

#define SLIST_LEN 1024

#define PORT 12345

struct server {
  char* ip;
  int dead;
  int refs;
};

struct server_list {
  struct server* value;
  struct server_list* next;
};

static struct server_list* known_servers;

// opens a socket to listen to and handle remote join requests
// returns 0 if successful, 1 if failure
int start_listener();

// Sends a list of servers to the specified socket
int send_list(int);

// Request a list of servers from host
// returns 0 if successful, 1 if failure
int request_list(struct server*);

// Add a server after server_list, keep teh stuff afterwards intact
int add_server(struct server*, struct server_list*);

// removes a server from the server list specified. Also sets the server's dead var to true
// 0 if successful, 1 if failure.
int rem_server(struct server*, struct server_list*);

// inform a server that we're living or dead.
// 0 if success
int inform(struct server*, char* type);

// Send a function to a server for evaluation
// 0 if success
int send_function(struct function*, struct server*);

// End this server, and notify all known servers of it's passing
// Memory and such will get freed here.
int die();
#endif
