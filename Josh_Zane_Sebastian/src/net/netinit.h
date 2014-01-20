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

#define REQUEST_LIST "REQLIST"

#define SLIST_LEN 1024

#define PORT 12345

struct server {
  char* ip;
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

// End this server, and notify all known servers of it's passing
// Memory and such will get freed here.
int die();
