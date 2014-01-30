#include "headers.h"
void createdeck();
void shuffle(char** deckofcards);
void hit(char ** deckofcards, int turncounter);
void stand(int turncounter);
void doubledown(char ** deckofcards, int turncounter);
void split(char ** deckofcards, int turncounter);
int gameSteup();

char **deck;
//Suit key: D = Diamonds, C = Clubs, H = Hearts, S = Spades
char *suit[5]= {"D","C","H","S"};
//Card key: A = Ace, J = Jack, Q = Queen, K = King
char *card[14]= {"A","2","3","4","5","6","7","8","9","10","J","Q","K","\0"};
int counter = 52;
int turncounter = 0;
char* buffer = (char*)malloc(sizeof(char)*256);

//makes a deck of 52 cards and shuffles it
void createdeck () {
  deck = malloc(53 * sizeof(char*));
  int i = 0;
  for (i;i < 53; i++) {
    deck[i] = malloc (sizeof(char*));
  }
  int j = 0;
  int k;
  for(j;j < 4;j++) {
    k = 0;
    for (k; k < 13; k++) {
      strcpy(deck[(j * 13) + k],suit[j]);
      strcat(deck[(j * 13) + k],card[k]);
    }
  }
  shuffle(deck);
  return;
}

//shuffles the deck using a seed for rand() of the time
void shuffle (char ** deckofcards) {
  int m = 51;
  for (m; m >= 0; --m) {
    srand(time(NULL));
    int r = rand()%(m+1);
    char* t = deck[m];
    deck[m] = deck[r];
    deck[r] = t;
  }
  return;
}

void hit(char ** deckofcards, int turncounter) {
  //give deck[counter] to client[turn counter]
  deck[counter] = "00";
  counter--;
  return;
}

void stand(int turncounter) {
  //server will stop prompting client[turn counter]
  return;
}

void doubledown(char ** deckofcards, int turncounter) {
  //give deck[counter] to client[turn counter]
  deck[counter] = "00";
  counter--;
  stay(turncounter);
  return;
}

void split(char ** deckofcards, int turncounter) {
  //give deck[counter] to client[turn counter]
  deck[counter] = "00";
  counter--;
  //give deck[counter] to client[turn counter]
  deck[counter] = "00";
  counter--;
  return;
}

int gameSetup() {
  printf("Attempting to make deck\n");
  createdeck();
  int p = 0;
  int l;
  for (p; p < 4; p++) {
    l = 0;
    for (l; l < 13; l++) {
      printf("%s ", deck[(p * 13) + l]);
    }
    printf("\n");
  }    
  printf("Deck created\n");
 
  while (1) {

    //read buffer from client[turncounter]   

    if (strcmp(buffer,"hit") == 0) {
      hit(deck, turncounter);
      if (/*client's hand is greater than 21*/) { 
	turncounter++;
	//set client's boolean BUST to true
      }
	
    }
    if (strcmp(buffer,"stand") == 0) {
      stand(turncounter);
      turncounter++;
      //set client boolean READY to true (meaning they are ready to show) 
    }
    
    if (strcmp(buffer,"double down") == 0) {
      doubledown(deck,turncounter);
      if (/*client's hand is greater than 21*/)
	//set client's boolean BUST to true
      turncounter++;
    }

    if (strcmp(buffer,"split") == 0) {
      split(deck, turncounter);
      //client will be prompted to work on both (this is gonna be hard to do)
    }

    if (turncounter >= 4) {
      //will compare all the clients with BUST = false and declare the highest the winner
      //ask to play again or end
    }

  return;
}


  /*
    this will most likely be included in the server file.
     client will have a char **hand, and two booleans READY & BUST, int turncounter (from 0-3) 
     Maybe an int money (otherwise, get rid of doubledown)
     Each client will keep playing until turncounter is changed. Once turncounter is equal than number of clients, endgame will commence. 
  */
    
