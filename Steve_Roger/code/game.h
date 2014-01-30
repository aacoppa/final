#define HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "util.h"

#define MAX_LEN 64
#define SEM_KEY 24601
#define SHM_KEY 10641
#define SEM_ID 0
#define SHM_ID 1
#define WKP "mario"


#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

/*
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
};*/
