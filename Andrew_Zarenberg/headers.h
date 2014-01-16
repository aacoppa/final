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



#define MAX_LEN 64
#define SEM_KEY 24601
#define SHM_KEY 10642
#define SEM_ID 0
#define SHM_ID 1
#define WKP "mario"
#define MASTER_PIPE "master_pipe"
#define DEFAULT_LIVES 5
#define SERVER_PORT 27643

#define SERVER_IP "149.89.150.105"




struct GAME_MEM {
  int score;
  int lives;
};
