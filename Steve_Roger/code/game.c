#include "game.h"

void init(); // initialize game or start from last save
int run();
void sighandler(int);
void prompt();
int getInput(); // gets the input

char *l = NULL;
size_t linecap = 0;

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
	/******* MAIN SECTION *******/

	loop {
		// add intro (backstory)
		printf("What is your name?\n");
		getInput();
		if (strcmp(l, "Roger") == 0 || strcmp(l, "Steve") == 0) {
			printf("So you are Nobleman %s.\n", l);
		} else {
			printf("So you are Peasant %s.\n", l);
			printf("I've heard much about your failed attempts to produce $$$$$ (profits)\n");
		}
	}

	free(l);
	return 0;
}

int getInput() {
	prompt();
	l = NULL;
	linecap = 0;
	if (getline(&l, &linecap, stdin) != -1) {
		*strchr(l, '\n') = '\0';
		if (strcmp(l, "exit") == 0) {
			exit(EXIT_SUCCESS);
		} else {
			printf("Debug: %s\n", l);
			return 0;
		}
	}
	return -1;
}

void prompt() {
	printf("> ");
}

void sighandler(int signo) {
	if (signo == SIGINT) {
		printf("\n");
		prompt();
		return;
	}
	if (signo == SIGSEGV) {
		printf("Segmentation fault\n");
	}
	exit(EXIT_FAILURE);
}
