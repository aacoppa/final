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
char save = 0;
char password[20];

int main() {
	setbuf(stdout, NULL);

	if (signal(SIGINT, sighandler) == SIG_ERR) {
		err("SIGINT ERR");
	}

	printf("\n");
	printColor("Welcome to Conquest of the Nigerian Prince v1!\n", C_GREEN);
	printColor("Type quit to exit the game\n\n", C_CYAN);

	init();

	return run();
}

void init() {
	generatePassword(password, sizeof(password));
	rmdir("files");
	saveExists(); // create files
	createDevices();
	chdir("files");
}

int run() {
	/******* MAIN SECTION *******/

	switch(save) {
		case 'a':
			goto a;
		case 'b':
			goto b;
		case 'c':
			goto c;
	}

	// add intro (backstory)
start:
	save = 0;
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
		printf("But that would be no fun");
	} else {
		printf("So you are Peasant %s.\n", l);
		printf("I've heard much about your failed attempts to produce $$$$$\n");
		printf("No worries though");
	}
	printf(" because earlier today, you received an email from trusted Nigerian Prince, %s.\n", PRINCE_NAME);
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
	printf("\n\nREST ASSURED THAT THIS IS LEGIT,\n");
	printf(C_MAGENTA "\nPrince %s  ʅ༼ຈل͜ຈ༽ʃ\n\n" C_RESET, PRINCE_NAME);
	typeToContinue();
a: save = 'a';
	printf("You have some choices " C_CYAN "(type a, b, or c)" C_RESET ":\n"
		"\tA) Give Jamal your bank info. (ಠ_ಠ)\n"
		"\tB) I've never seen anything that looked so much like a scam. Jamal probably isn't even a real person.\n"
		"\tC) I respect the name Jamal just as I respect people of all races, but I believe this is illegitimate. I humbly refuse your offer.\n"
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
b: save = 'b';
		printf("Too bad Jamal is a master hacker. Today is not your day (•︵•)\n");
		printf("He somehow sshs into your system and tries to crack your encrypted sudo password in order to rm -rf your 700TB of pornography.\n");
		hidePrince();
		printf("Good thing he'll never find out that it's stored in '/Not Porn'\n");
		printf("\n");
		typeToContinue();
		printf("You find out that your mom helped him ssh in because she though he was a friend from school.\n");
		printf("Goddammit mom... (ノಠ益ಠ)ノ彡┻━┻\n");
		printf("Jamal manages to find the encrypted file with all your credentials.\n");
		printf("He creates a secure shack in your file system so that he can have a place to stay while he tries to brute force your password.\n");
		printf("You must find and delete him before it is too late!\n\n");

		goToRoot();
		printColor("You are now at the root.\n", C_CYAN);
		printColor("Use cd to navigate the system.\n", C_CYAN);
		printColor("Use pwd to see the current directory.\n", C_CYAN);
		loop {
			getInput(true);
			if (isPrinceHere()) {
				break;
			}
		}
c: save = 'c';
		printf("\n");
		printColor("Use ls to see the files in the current directory.\n", C_CYAN);
		printColor("Use rm to remove files.\n", C_CYAN);
		loop {
			getInput(true);
			if (!strcasecmp(l, "rm " PRINCE_NAME)) {
				printf("You don't have the permissions to delete Jamal.\n");
				printf("What kind of voodoo magic is this...?\n");
				printf("...Perhaps you have to have sudo permissions.\n");
			} else if (!strcasecmp(l, "sudo rm " PRINCE_NAME)) {
				printf("Jamal forgot to securely store the password in his excitement.\n");
				printf("It's probably lying around here somewhere in these folders.\n");
				hidePassword();
				printf("Enter the password:\n");
				goToRoot();
				printColor("You are now at the root.\n", C_CYAN);
				break;
			}
		}
d: save = 'd';
		loop {
			getInput(true);
			if (!strcmp(l, password)) {
				break;
			} else {
				printf("The password is incorrect.\n");
			}
		}

		printf("Make a choice " C_CYAN "(type a or b)" C_RESET ":\n"
			"\tA) Actually delete Jamal.\n"
			"\tB) Banish Jamal from your filesystem.\n"
		);
		do {
			getInput(true);
		} while (strcasecmp(l, "a") && strcasecmp(l, "b"));
		if (!strcasecmp(l, "a")) {
			printf("You managed to destroy Jamal from existence, but he managed to delete 99%% of your porn stash.\n");
			printf("There is no happy ending. Better luck in real life.\n");
		} else if (!strcasecmp(l, "b")) {
			printf("You see news of Jamal ruining the lives of other good citizens, but at least he's too scared to mess with you again.\n");
			printf("He still managed to delete 99%% of your porn stash though. You vow to get revenge.\n");
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
		// if not nothing entered
		if (!strcasecmp(l, "pwd")) {
			printf("%s\n", relativeDir());
		} else if (!strcasecmp(l, "ls")) {
			ls();
		} else if (!strncasecmp(l, "cd", 2)) {
			if (!strncmp(l + 3, "..", 2) && !strncmp(relativeDir(), "/", 2)) {
				printf("You are already in the root dir...\n");
			} else if (chdir(l + 3) != -1) {
				char * rel = relativeDir();
				printf("%s\n", rel);
				if (save == 'b') {
					if (isPrinceHere()) {
						printf("Jamal is a sneaky *** ************. You found him though. Good job!\n");
						break;
					} else if (!strcmp(rel, "/Computer/Documents/Work")) {
						printf("This folder is serious business; he won't be in here.\n");
					} else if (!strcmp(rel, "/Computer/Documents/School")) {
						printf("Jamal has an aversion to school.\n");
					} else if (!strcmp(rel, "/Computer/Documents/Not Porn")) {
						printf("There's no way Jamal is in here yet. It is protected by state of the art 2^100 bit encryption\n");
					} else if (!strcmp(rel, "/Computer/Games/Club Penguin")) {
						printf("It doesn't look like Jamal is sliding around on the ice here.\n");
					} else if (!strcmp(rel, "/Computer/Games/League of Legends")) {
						printf("Jamal is Diamond I, he has no interest in your peasantry.\n");
					} else if (!strcmp(rel, "/Computer/Games/Runescape")) {
						printf("Jamal has 2,147,483,648 gp, 1 more coin than is programmatically possible. That's a Nigerian Prince for you.\n");
					} else {
						printf("Jamal isn't here.\n");
					}
				} else if (save == 'c') {
					if (isPasswordHere()) {
						printf("You found the password to end Jamal's whole family tree:\n");
						printf("%s\n", password);
						save = 'd';
						break;
					} else {
						printf("Password not detected here.");
					}
				}
			} else {
				perror("cd");
				printColor("Type ls to list possible directories.\n", C_CYAN);
			}
		} else if (!denyEmpty || (l != NULL && *l != '\0')) {
			break;
		}
	} while (read != -1);

	if (!strcasecmp(l, "quit")) {
		exit(EXIT_SUCCESS);
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
