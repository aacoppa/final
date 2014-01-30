#include "game.h"

void init(); // initialize game or start from last save
int run();
void sighandler(int);
void typeToContinue();
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

	printf("\n");
	printColor("Welcome to Conquest of the Nigerian Prince v1!\n", C_GREEN);
	printColor("Type exit to quit the game\n\n", C_CYAN);

	//printf("%s\n",relativeDir());

	init();
	return run();
}

void init() {
	saveExists(); // create files
	createDevices();
}

int run() {
	/******* MAIN SECTION *******/

	char save = 0;

	switch(save) {
		case 'a':
			goto a;
		case 'b':
			goto b;
	}

	// add intro (backstory)
start:
	printf("What is your name?\n");
	do {
		getInput(false);
		if (*l == '\0') {
			printf("Please enter a name.\n");
		}
	} while (*l == '\0');
	name = strdup(l);
	if (!strcasecmp(l, "Roger") || !strcasecmp(l, "Steve")) {
		printf("So you are Nobleman %s.\n", l);
		printf("There is no one in this universe as awesome as you.\nYou should just win the game right now.\n");
		printf("But that would be no fun because");
	} else {
		printf("So you are Peasant %s.\n", l);
		printf("I've heard much about your failed attempts to produce $$$$$\n");
		printf("No worries though because");
	}
	printf("earlier today, you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
	printf("\n");
	typeToContinue();
	printf("From: <princejamal2401@aol.com>\n\n");
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
	printf("\nPrince %s\n\n", PRINCE_NAME);

a:
	printf("You have some choices " C_CYAN "(type a, b, or c)" C_RESET ":\n"
		"\tA) Give Jamal your bank info. (ಠ_ಠ)\n"
		"\tB) I've never seen anything that looked so much like a scam. Jamal probably isn't even a real person.\n"
		"\tC) I respect the name Jamal just as I respect people of all races, but I believe this is illegitimate.\n"
		"\t   I humbly refuse your offer.\n"
	);

	do {
		getInput(true);
	} while (strcasecmp(l, "a") && strcasecmp(l, "b") && strcasecmp(l, "c"));

	if (!strcasecmp(l, "a")) {
		printf("Learn to English. I've never seen an email as fake as this. \"Prince\" Jamal just stole all your money.\n");
		goto lose;
	} else if (!strcasecmp(l, "b")) {
		printf("That's racist. Jamal is just as much a real person as you. YOU think he's not a person just because he's black?!\n");
		goto lose;
	} else {
b:
		printf("Too bad Jamal is a master hacker. Today is not your day (•︵•)\n");
		printf("He somehow sshs into your system and tries to crack your encrypted sudo password in order to rm -rf your 700TB of pornography.\n");
		printf("Good thing he'll never find out that it's stored in '/Not Porn'\n");
		printf("\n");
		typeToContinue();
		printf("You find out that your mom helped him ssh in because she though he was a friend from school.\n");
		printf("Goddammit mom... (ノಠ益ಠ)ノ彡┻━┻\n");
		printf("Jamal manages to find the encrypted file with all your credentials.\n");
		printf("He creates a secure shack in your file system so that he can have a place to stay while he tries to brute force your password.\n");
		printf("You must find him before it is too late!\n\n");
		printColor("Use cd to navigate the system.\n", C_CYAN);
		printColor("Use pwd to see the current directory.\n", C_CYAN);
		loop {
			getInput(true);
		}
	}

	free(l);
	return 0;

lose:
	printf("You lose\n");
	printf("Start over?\n");
	loop {
		getInput(true);
		if (!strcasecmp(l, "yes") || !strcasecmp(l, "y")) {
			goto start;
		} else if (!strcasecmp(l, "no") || !strcasecmp(l, "n")) {
			return 0;
		} else {
			printColor("Please enter yes or no.\n", C_CYAN);
		}
	}
}

// true to loop if the input is empty
int getInput(int denyEmpty) {
	l = NULL;
	linecap = 0;
	int read;

	do {
		prompt();
		read = getline(&l, &linecap, stdin);
		*strchr(l, '\n') = '\0'; // delete \n char
		// if nothing entered
		if (denyEmpty && (l == NULL || *l == '\0')) {
			continue;
		} else {
			break;
		}
	} while (read != -1);

	if (!strcasecmp(l, "exit")) {
		exit(EXIT_SUCCESS);
	} else if (!strcasecmp(l, "pwd")) {
		printf("%s\n", relativeDir());
		l = NULL;
	} else {
		//printf(C_RED "Debug: %s\n" C_RESET, l);
	}

	return read;
}

void typeToContinue() {
	printColor("Type anything to continue.\n", C_CYAN);
	getInput(false);
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
