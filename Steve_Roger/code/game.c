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
start:
	printf("What is your name?\n");
	getInput(0);
	name = strdup(l);
	if (!strcasecmp(l, "Roger") || !strcasecmp(l, "Steve")) {
		printf("So you are Nobleman %s.\n", l);
		printf("There is no one in this universe as awesome as you.\nYou should just win the game right now.\n");
		printf("But that would be no fun tho becase earlier today, you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
	} else {
		printf("So you are Peasant %s.\n", l);
		printf("I've heard much about your failed attempts to produce $$$$$\n");
		printf("No worries tho because earlier today you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
	}
	printf("Type anything to continue.\n");
	getInput(1);
	printf("\nFrom: <princejamal2401@aol.com>\n\n");
	printf("DEAR %s,\n\n", name);
	printf(
		"DON'T BE AFRAID THAT I MYSTERIOUSLY KNOW YOUR NAME. I AM A NIGERIAN PRINCE AFTER ALL, AND YOU ARE MY LONG LOST SON."
		" THE DIVIDED STATES OF STATEN ISLAND ARE TRYING TO STAGE A COUP TO OVERTHROW MY KINGDOM."
		" THEY WANT TO SEIZE MY ASSETS AMOUNTING TO ONE HUNDRED BILLION $$$$ ($100,000,000,000) AND USE IT TO PAY OFF THEIR DEBTS TO CHINA."
		" I CANNOT ALLOW THAT TO HAPPEN. I MUST TRANSFER THE MONEY TO A BANK OUTSIDE OF NIGERIA."
		" UNFORTUNATELY ALL THE ROYAL FAMILY BANK ACCOUNTS OUTSIDE OF THE COUNTRY WERE FROZEN BY HUGH JAZZ, COMMANDER OF THE SOMO OPERATION FORCES, WHO IS LEADING THE TAKEOVER OF THE NIGERIA."
		"\n\nSEND YOUR BANK INFORMATION IMMEDIATELY SO THAT I CAN BEGIN THE TRANSFER OF FUNDS. "
	);
	printf("\n\nREST ASSURED THAT THIS IS LEGIT,");
	printf("\nPrince %s\n", PRINCE_NAME);
a:
	printf("You have some choices (type a or b):\n"
		"\tA) Give Jamal your bank info.\n"
		"\tB) I've never seen anything that looked so much like a scam.\n"
		"\t   Jamal probably isn't even a real person.\n"
	);

	do {
		getInput(1);
	} while (!strcasecmp(l, "a") && !strcasecmp(l, "b"));

	if (!strcasecmp(l, "a")) {
		printf("Learn to English. I've never seen an email as fake as this. \"Prince\" Jamal just stole all your money. You lose.\n");
		return 0;
	} else {

	}

	free(l);
	return 0;
}

int getInput(int allowEmpty) {
	l = NULL;
	linecap = 0;
	int read;

	do {
		prompt();
		read = getline(&l, &linecap, stdin);
		*strchr(l, '\n') = '\0'; // delete \n char
		// if nothing entered
		if (!allowEmpty && (l == NULL || *l == '\0')) {
			continue;
		} else {
			break;
		}
	} while (read != -1);

	if (strcmp(l, "exit") == 0) {
		exit(EXIT_SUCCESS);
	} else {
		//printf(C_RED "Debug: %s\n" C_RESET, l);
	}

	return read;
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
