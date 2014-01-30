#include "game.h"

char sigintCount = 0; // number of times ctrl + c has been pressed (2 times to quit)

void init(); // initialize game or start from last save
int run();
void sighandler(int);

int main() {
	setbuf(stdout, NULL);

	if (signal(SIGINT, sighandler) == SIG_ERR) {
		err("SIGINT ERR");
	}

	// show welcome message?
	printf("Welcome\n");

	init();
	return run();
}

void init() {
    int save = saveExists();
    if (save == 0) {
        // save doesn't exist, build files and shit
        createDevices();
    }
}

int run() {
	char *line = NULL;
	size_t linecap = 0;
    
start:
	goto prompt;
    
    
	free(line);
	return 0;
    
prompt:
	printf("> ");
	// read from stdin
	if (getline(&line, &linecap, stdin) != -1) {
		printf("%s", line);
	}
	sigintCount = 0;
	goto start;
}

void sighandler(int signo) {
	if (signo == SIGINT) {
		if (sigintCount == 0) {
			sigintCount++;
			printf("\nPress ^C again to quit\n> ");
			return;
		} else {
			printf("\nGoodbye\n");
		}
	} /*else if (signo == SIGSEGV) {
		printf("Segmentation fault\n");
	}*/
	exit(EXIT_FAILURE);
}
