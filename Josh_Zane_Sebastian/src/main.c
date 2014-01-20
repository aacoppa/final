#include "net/netinit.h"

int main() {
  #if 0
    start_listener();
    while(1);
  #else
    struct server* host;
    host->ip = "127.0.0.1";
    request_list(host);
  #endif
  die();
  return 0;
}
