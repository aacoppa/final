#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <signal.h>
#include <assert.h>
#include <netinet/in.h>
#include <time.h>

#define MAX_PLAYERS 4
#define SERVER_PORT 27046
