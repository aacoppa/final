#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

char **deck;
//Suit key: D = Diamonds, C = Clubs, H = Hearts, S = Spades
char *suit[5]= {"D","C","H","S"};
//Card key: A = Ace, J = Jack, Q = Queen, K = King
char *card[14]= {"A","2","3","4","5","6","7","8","9","10","J","Q","K","\0"};

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
  return;
}

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

int main() {
  printf("Attempting to make deck\n");
  createdeck();
  shuffle(deck);
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
  return;
}

  
    
