#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <process.h>
#include <ctype.h>

#define ASCII 48

int wins;
int losses;
int money = 100;
int bet;
int playerval;
int dealerval;
int playerhandsize=0;
int dealerhandsize=0;
int playeraces=0;
int dealeraces=0;
int playerdone=0;
int dealerdone=0;
int done;

typedef struct Card{
	char faceval[2];
	char suit;
	int trueval;
	int isAce;
}Card;

char* ready;
char* action;
char* restart;
Card playerhand[11];
Card dealerhand[11];

char randsuit();
Card randval();
char randcard();
void dealto(char* who);
char* printCard(Card c);
void printhand(char* who);
void game();

//int i = '8' - ASCII;

int main(){
	printf("Welcome to BlackJack!\n");
	printf("Created by Brian Yee, Period 4 Systems Class\n");
	printf("\n");
	printf("\n");
	printf("You start with $100 and you may bet as much as you want, as long as it's something!\n");
	printf("Game will keep track of your wins and losses for the current session.\n");
	game();
}

char randsuit(){
	int r;
	srand(time(NULL));
	r = (int)rand()%4+1;
	if (r == 1) return 'S';
	else if (r == 2) return 'H';
	else if (r == 3) return 'C';
	else return 'D';
}

Card randval(){
	Card temp;
	int v;
	srand(time(NULL));
	v = (int)rand()%13+2;
	if(v < 11){
		sprintf(temp.faceval,"%d",v);
		temp.suit = randsuit();
		temp.trueval = v;
		temp.isAce = 0;
	}
	else if(v == 11){
		strcpy(temp.faceval,"J");
		temp.suit = randsuit();
		temp.trueval = 10;
		temp.isAce = 0;
	}
	else if(v == 12){
		strcpy(temp.faceval,"Q");
		temp.suit = randsuit();
		temp.trueval = 10;
		temp.isAce = 0;
	}
	else if(v == 13){
		strcpy(temp.faceval,"K");
		temp.suit = randsuit();
		temp.trueval = 10;
		temp.isAce = 0;
	}
	else{
		strcpy(temp.faceval,"A");
		temp.suit = randsuit();
		temp.trueval = 11;
		temp.isAce = 1;
	}
	return temp;		
}

void dealto(char* who){
	Card temp = randval();
	if(who == "dealer"){
		if(temp.isAce == 1)
			dealeraces++;
		dealerhand[dealerhandsize] = temp;
		dealerhandsize++;
		dealerval = dealerval + temp.trueval;
	}
	else if(who == "player"){
		if(temp.isAce == 1)
			playeraces++;
		playerhand[playerhandsize] = temp;
		playerhandsize++;
		playerval = playerval + temp.trueval;
	}
}

char* printCard(Card c){
	char* temp;
	printf("1");
	char* face;
	printf("2");
	//strcpy(face, c.faceval);
	printf("3");
	char* suit;
	printf("4");
	//strcpy(suits, c.suit);
	printf("5");
	//strcat(temp,strcat(face,suit));
	printf("6");
	return temp;
}

void printhand(char* who){
	int i;
	if(who == "player"){
		printf("This is your hand: ");
		for(i = 0; i < playerhandsize; i++){
			printf("[%s] ", playerhand[i]);
		}
	}
	else if(who == "dealer"){
		printf("This was the dealer's hand: ");
		for(i = 0; i < dealerhandsize; i++){
			printf("[%s] ", dealerhand[i]);
		}
	}
	printf("\n");
}

void game(){
	printf("Your current record is %i Wins, and %i Losses\n", wins, losses);
	if(money == 0){
		printf("Sorry, but you're broke now. Restart for another run.\n");
		exit(0);
	}
	else{
		char buff[256];
		printf("You have $%i. How much would you like to bet? ",money);
		fgets(buff, sizeof(buff), stdin);
		bet = atoi(buff);
		while(bet == 0 || bet > money){
			if(bet == 0){
				printf("Don't be so cheap, bet something!\n");
				printf("How much money would you like to bet? ");
				fgets(buff, sizeof(buff), stdin);
				bet = atoi(buff);
			}
			else if(bet > money){
				printf("YOU DONT HAVE THAT MUCH MONEY THOUGH!!!\n");
				bet = 0;
				printf("How much money would you like to bet? ");
				fgets(buff, sizeof(buff), stdin);
				bet = atoi(buff);
			}
		}
		printf("Okay, you have bet %i\n\n\n",bet);
		dealto("dealer");
		dealto("dealer");
		printf("This is the dealer's hand: [%s] [??]\n", printCard(dealerhand[0]));
		dealto("player");
		dealto("player");
		printf("This is your hand: [%s] [%s]\n", printCard(playerhand[0]),printCard(playerhand[0]));
		while(1){
			while(!dealerdone){
				if(dealerval < 17){
					dealto("dealer");
				}
				else if(dealerval > 17 && dealeraces > 0){
					dealerval = dealerval - 10;
					dealeraces--;
				}
				else if(dealerval > 17 && dealeraces == 0){
					dealerdone++;
				}
			}
			while(!playerdone){
				if(playerval < 22){

					printf("what would you like to do?(hit/stay)");
					fgets(action, sizeof(action), stdin);
					if(action == "hit" || action == "Hit"){
						dealto("player");
					}
					else if(action == "stay" || action == "Stay"){
						playerdone++;
					}
				}
				else if(playerval > 22 && playeraces > 0){
					playerval = playerval - 10;
					playeraces--;
				}
				else if(playerval > 22 && playeraces == 0){
					printf("Sorry! You went over!\n");
					printhand("player");
					printhand("dealer");
					done++;
					losses++;
					money = money - bet;
					bet = 0;
					playerdone++;
					break;
				}
				else if(playerval == 21){
					printf("BLACKJACK!!! You hit 21!\n");
					printhand("player");
					printhand("dealer");
					done++;
					wins++;
					money = money + bet;
					bet = 0;
					playerdone++;
					break;
				}
			}
		}
		if(done == 0){
			done++;
			if(playerval > dealerval){
				printf("Congratulations! You won with a score of %i over the dealer's %i\n", playerval, dealerval);
				printf("You won $%i\n",bet);
				printhand("player");
				printhand("dealer");
				money = money + bet;
				bet = 0;
				wins++;
			}
			else if(playerval < dealerval){
				printf("Sorry, but you're a loser. You lost with a score of %i to the dealer's %i\n", playerval, dealerval);
				printf("You lost $%i\n",bet);
				printhand("player");
				printhand("dealer");
				money = money - bet;
				bet = 0;
				losses++;
			}
			else if(playerval == dealerval){
				printf("You didn't lose, but you didn't win either. Too bad life isn't fair, so you actually lost\n");
				printf("You both had a score of %i\n", playerval);
				printf("You lost $%i\n",bet);
				printhand("player");
				printhand("dealer");
				money = money - bet;
				bet = 0;
				losses++;
			}
		}
		printf("Would you like to play again?(Y/N) ");
		fgets(restart, sizeof(restart), stdin);
		if(restart == "Y" || restart == "y" || restart == "Yes" || restart == "yes"){
			game();
		}
		else if(restart == "N" || restart == "n" || restart == "No" || restart == "no"){
			printf("You ended with a record of %i wins, %i losses, with $%i left.");
			exit(0);
		}
	}
}