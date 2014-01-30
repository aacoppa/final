#include "netinit.h"

int start_listener() {
  if (known_servers == NULL) {
    known_servers = calloc(1, sizeof(struct server_list));
  }
  int socket_id, socket_client;
  char buffer[256];

  struct sockaddr_in server;
  socklen_t socket_length;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);
   
  // IPv4, yo
  server.sin_family = AF_INET;
  // Listen to everything coming in.
  server.sin_addr.s_addr = INADDR_ANY;
  // Select the coolest port
  server.sin_port = htons(PORT);

  // Bind the socket
  bind(socket_id, (struct sockaddr *)&server, sizeof(server));

  if (fork() == 0) {

    // Listen to the socket, it has strong feelings about a great many topics
    listen(socket_id, 0);

    // Accept connections forever
    while (1) {
      printf("Waiting for conn\n");

      socklen_t l = sizeof(server);

      socket_client = accept(socket_id, (struct sockaddr *)&server, &l);

      printf("Accpeted conn: %d\n", socket_client);

      // Read what the client has said to us
      read(socket_client, buffer, sizeof(buffer));

      printf("Recieved: %s\n", buffer);

      if (strcmp(buffer, REQUEST_LIST) == 0) { // If they want our number, let 'em have it

        send_list(socket_client);
        
        continue; // Jump to the next iteration.
      } else if (strcmp(buffer, REQUEST_INFORM) == 0) { // They're letting us know they exist, how sweet. Unless they're dying. That's not sweet.
        read(socket_client, buffer, sizeof(buffer)); // Find out what they want to tell us
        printf("INFORMMED: %s\n", buffer);
        if (strcmp(buffer, INFORM_LIVE) == 0) {
          // Add the client to our list
          char cli[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &server.sin_addr, cli, INET_ADDRSTRLEN);

          printf("remote address: %s\n", cli);
          struct server client;
          /*strcpy(client.ip, cli);*/
          client.ip = cli;
          printf("client.ip: %s\n", client.ip);
          add_server(&client, known_servers);
          printf("Done adding\n");
          continue; // Jump to the next iteration.
        } else if (strcmp(buffer, INFORM_DEAD) == 0) {
          char cli[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &server.sin_addr, cli, INET_ADDRSTRLEN);

          printf("remote address: %s\n", cli);
          struct server client;
          strcpy(client.ip, cli);
          rem_server(&client, known_servers);
        } else if (strcmp(buffer, REQUEST_FN) == 0) {
          struct function* fn = calloc(1, sizeof(struct function));
          read(socket_client, buffer, sizeof(buffer));
          if (strcmp(buffer, FUNCTION_ARGS) != 0) {
            fprintf(stderr, "Fuck, %s is not %s\n", buffer, FUNCTION_ARGS);
          }
          read(socket_client, buffer, sizeof(buffer));
          fn->numargs = atoi(buffer);
          fn->args = calloc(numargs, sizeof(struct symbol));
          int i;
          for (i = 0; i < fn->numargs; ++i) {
            read(socket_client, buffer, sizeof(buffer));
            fn->args[i]->typeid = buffer[0];
            read(socket_client, buffer, sizeof(buffer));
            fn->args[i]->val = buffer;
          }
          read(socket_client, buffer, sizeof(buffer));
          fn->definition = buffer;
          //TODO: Do something with this function
        }
      }
    }
    printf("Closing conn\n");
    close(socket_id);
  }
  return 0;
}

int send_list(int socket_client) {
  if (known_servers == NULL) {
    known_servers = calloc(1, sizeof(struct server_list));
    known_servers->value = calloc(1, sizeof(struct server));
  }
  // Respond with list of servers, seperated by commas
  struct server_list* curr;
  char slist[SLIST_LEN]; // buffer for our server list to be stored in
  strcpy(slist, ",");
  size_t space_remaining = sizeof(slist);
  printf("Preparing server list\n");
  for (curr = known_servers; curr != 0; curr = curr->next) {
    space_remaining = sizeof(slist) - strlen(slist);
    if (curr->value != NULL && space_remaining >= sizeof(curr->value->ip)) { // If we have room
      printf("Adding server: %s\n", curr->value->ip);
      strcat(slist, curr->value->ip);
      if (curr->next != 0) {
        strcat(slist, ",");
      }
    } else { //We're out of room in our buffer, let's send it even though it's technically incomplete.
      printf("Well I'll be, either we ran out of all %lu bytes in our sending string or this was our first run because Josh is too lazy to do multiple if statements\n", sizeof(slist));
      break;
    }
  }

  // Send the client the list
  printf("Sending server list: %s\n", slist);
  write(socket_client, slist, strlen(slist));
  return 0;
}

int request_list(struct server* host) {
  if (known_servers == NULL) {
    known_servers = calloc(1, sizeof(struct server_list));
  }
  struct server_list* curr = known_servers;
  char buffer[256];
  struct sockaddr_in sock;
  int socket_id;
  char slist[SLIST_LEN];

  socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // IPv4, yo
  sock.sin_family = AF_INET;
  
  // Convert the host's ip to something usable
  inet_aton(host->ip, &(sock.sin_addr));

  sock.sin_port = htons(PORT);

  // Establish connection
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  printf("Sending list request\n");
  // Send them a request for a list of servers
  write(socket_id, REQUEST_LIST, sizeof(REQUEST_LIST));

  printf("Waiting for list...\n");
  // Read their response into slist
  read(socket_id, slist, sizeof(slist));

  printf("Recieved server list: %s\n", slist);

  char* curr_str = slist;
  char* token;

  inform(host, INFORM_LIVE);

  if (strlen(slist) == 0) { // The other server didn't know of any servers, how sad
    return 0;
  }
  // For each server sent
  while ((token = strsep(&curr_str, ",")) != NULL) {
    curr->value = calloc(1, sizeof(struct server)); // allocate space
    curr->value->ip = token;
    curr->value->dead = 0; // liiive, liiive!
  
    // Inform curr that we exist
    inform(curr->value, INFORM_LIVE);

    curr->next = calloc(1, sizeof(struct server_list));
    curr = curr->next;
  }

  close(socket_id);

  return 0;
}


int add_server(struct server* new, struct server_list* list) {
  struct server_list* insert = calloc(1, sizeof(struct server_list));
  insert->value = new;
  if (list != NULL) {
    insert->next = list->next;
  }
  list->next = insert;
  new->dead = 0; // It's aliiiive
  return 0;
}

int inform(struct server* remote, char* type) {
  char buffer[256];
  struct sockaddr_in sock;
  int socket_id;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // IPv4, yo
  sock.sin_family = AF_INET;
  
  // Convert the host's ip to something usable
  inet_aton(remote->ip, &(sock.sin_addr));

  sock.sin_port = htons(PORT);

  // Establish connection
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  // Send them an inform for whatever we're talkin' bout.
  write(socket_id, REQUEST_INFORM, sizeof(REQUEST_INFORM));
  sleep(1); //Wait a teensy bit of time
  printf("Informing: %s\n", type);
  write(socket_id, type, sizeof(type));

  close(socket_id);
  
  return 0;
}

int rem_server(struct server* rem, struct server_list* list) {
  struct server_list* curr;
  // Go through our servers
  for (curr = list; curr->next != 0; curr = curr->next) {
    struct server* test = curr->next->value; // We want to test 1 forward, so we can relink the list
    if (test == rem || (strcmp(test->ip, rem->ip) == 0)) { // If they're identical pointers or if they have the same ip, that's a match!
      struct server_list* temp = curr->next->next; // store the next next value
      if (!test->refs) { // If there are no existing references to this server, free it off.
        free(test->ip);
        free(test);
      } else {
        test->dead = 1; // Otherwise, label it as dead so nobody want's to use it.
      }
      curr->next = temp;
      return 0; // We're done here.
    }
  }
  return 1; // We found nothing
}

int send_function(struct function* fn, struct server* dest) {
  /*
   * Functions get sent as:
   *  REQUEST_FN
   *  sleep
   *  ARGS
   *  sleep
   *  numargs
   *  sleep
   *  arg1
   *  sleep
   *  arg2
   *  sleep
   *  ...
   *  argn
   *  sleep
   *  FUNCTION_CONTENTS
   *  sleep
   *  function->text
   */
  char buffer[256];
  struct sockaddr_in sock;
  int socket_id;
  int i;

  socket_id = socket(AF_INET, SOCK_STREAM, 0);

  // IPv4, yo
  sock.sin_family = AF_INET;
  
  // Convert the host's ip to something usable
  inet_aton(dest->ip, &(sock.sin_addr));

  sock.sin_port = htons(PORT);

  // Establish connection
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));

  write(socket_id, REQUEST_FN, sizeof(REQUEST_FN));
  sleep(1);
  write(socket_id, FUNCTION_ARGS, sizeof(FUNCTION_ARGS));
  sleep(1);
  write(socket_id, fn->numargs, sizeof(fn->numargs));
  sleep(1);
  for (i = 0; i < fn->numargs; ++i) {
    write(socket_id, fn->args[i]->name, sizeof(fn->args[i]->name));
    write(socket_id, fn->args[i]->referant->typeid, sizeof(fn->args[i]->referant->typeid));
    write(socket_id, fn->args[i]->referant->val, sizeof(fn->args[i]->referant->val));
    sleep(1);
  }
  write(socket_id, FUNCTION_CONTENTS, sizeof(FUNCTION_CONTENTS));
  sleep(1);
  write(socket_id, fn->definition, sizeof(fn->definition));


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
    current = next;
  } while (current != 0);
  return 0;
}
