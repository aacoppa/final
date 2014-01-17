#include "netinit.h"

int start_listener() {
  //TODO: Fork off a listening server.
  return 0;
}

int request_list(struct server* host) {
  known_servers = calloc(1, sizeof(struct server_list));
  known_servers->value = host; // Put our first known server at the top
  // TODO: Retrive server list from host.
  return 0;
}

int die() {
  struct server_list* current = known_servers;
  struct server_list* next;
  do {
    next = current->next;
    // TODO: Notify current of my passing.
    free(current->value);
    free(current); // Master has given current a sock! Current is FREE!
  } while (next != 0);
  return 0;
}
