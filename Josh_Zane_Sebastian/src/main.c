#include "net/netinit.h"

int main(int argc, char* argv[]) {
  printf("%d\n", argc);
  if (argc == 1) {
    start_listener();
    while(1);
  } else {
    struct server* host = calloc(1, sizeof(struct server));
    host->ip = "127.0.0.1";
    request_list(host);
  }
  die();
  return 0;
}
