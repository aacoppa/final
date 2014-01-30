#include "game.h"

#define PRINCE_NAME "Jamal Kareem Tijani"

void init(); // initialize game or start from last save
int run();
void sighandler(int);
void prompt();
int getInput(); // gets the input

char *l = NULL;
size_t linecap = 0;
char *name;

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

	// add intro (backstory)
	printf("What is your name?\n");
	getInput();
	name = strdup(l);
	if (strcasecmp(l, "Roger") == 0 || strcasecmp(l, "Steve") == 0) {
		printf("So you are Nobleman %s.\n", l);
		printf("There is no one in this universe as nomo as you.\nYou should just win the game right now.\n");
		printf("But that would be no fun tho becase earlier today, you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
	} else {
		printf("So you are Peasant %s.\n", l);
		printf("I've heard much about your failed attempts to produce $$$$$\n");
		printf("No worries tho because earlier today you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
	}
	printf("\n");
	printf("Dear %s,\n\n", name);
	printf("Don't be afraid that I mysteriously know your name. I am a Nigerian Prince after all and you are my long lost son.");
	printf(" The Divided States of Staten Island are trying to stage a coup to overthrow my kingdom.");
	printf(" They want to seize my assets amounting to $100,000,000,000 and use it to pay off their debts to China.");
	printf(" I cannot allow that to happen.\n");
	printf("\nPrince %s\n", PRINCE_NAME);

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
			printf(C_RED "Debug: %s\n" C_RESET, l);
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
