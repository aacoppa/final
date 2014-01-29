#include "header.h"

void sighandler(int);

void init(); // initialize game or start from last save

void sighandler(int signo) {
	if (signo == SIGINT) {
 		printf("Keyboard interrupt\n");
	} else if (signo == SIGSEGV) {
		printf("Segmentation fault\n");
	}
	exit(EXIT_FAILURE);
}

void init() {

}

int main() {
	if (signal(SIGINT, sighandler) == SIG_ERR) {
		err("SIGINT ERR");
	}

    init();

    while(1) {



    }


	return 0;
}
