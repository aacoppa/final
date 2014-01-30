#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>



#define NTHREADS 50
#define QUEUE_SIZE 5
#define BUFFER_SIZE 256
#define WHITE_SIZE 537
#define BLACK_SIZE 104
