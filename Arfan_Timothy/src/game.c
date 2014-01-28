#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "game.h"

/* method for resetting game loop */
void reset_game(Game * g){
    free(g);
    Game * game = malloc(sizeof(Game));
    init_game(game);
}

/* this method begins the game */
void init_game(Game * game){
    printf("Welcome to 5-card draw Poker with Monte Carlo simulations!\n");
    game->deck = malloc(sizeof(Deck));
    game->players = malloc(sizeof(Player) * PLAYER_SIZE);
    game->value = TOTAL_CASH;
    game->pot = 0;
    game->ante = ANTE_AMOUNT;
    game->player_count = PLAYER_SIZE;
    init_deck(game->deck);
    init_players(game->players, PLAYER_SIZE);
}

/* determines whether a user is eligible to play, if they can't ante up, they lose */
void ante_up(Game * game){
    int i, count;
    count = 0;
    for (i = 0; i < game->player_count; ++i){
        if (game->players[i].isActive){
            if ((game->players[i].amount - game->ante) < 0){
                printf("%s loses, not enough money to play\n", game->players[i].name);
                game->players[i].isActive = 0;
                game->players[i].hasFolded = 1;
                game->pot += game->players[i].amount; /* leftovers go into pot */
                game->players[i].amount = 0;
                count++;
                continue;
            }
            game->pot += game->ante; /* increase pot */
            game->players[i].amount -= game->ante; /* decrease player pot */
            printf("%s ante's %d, now has: %d chips\n", game->players[i].name, game->ante, game->players[i].amount);
        }
    }
    printf("Pot at: %d\n", game->pot);
}

/* called twice, once before exchange, once after */
void betting_round(Game * game, Player * players, int player_size){
    int i;
    i = 0;
    char * result = malloc(sizeof(char) * 10);
    int bet;
    int count;
    
    /* check to make sure everyone didn't fold */
    /* if everyone has folded */
    count = 0;
    for (i = 0; i < PLAYER_SIZE; ++i)
        if (players[i].isActive && players[i].hasFolded)
            count++;
    
    if (player_size - count != 1 && game->pot != TOTAL_CASH){ /* everyone has folded go straight to determining winner */
        /* clear previous better */
        for (i = 0; i < player_size; ++i)
            players[i].isBetter = 0;
        
        /* determine new better, randomly */
    find_better:
        i = rand() % 4;
        if (!players[i].isActive || players[i].hasFolded)
            goto find_better;
        players[i].isBetter = 1;
        
        /* make bets */
        for (i = 0; i < player_size; ++i) {
            if (players[i].isComputer && players[i].isBetter){
                bet = rand() % players[i].amount;
                printf("%s bet %d\n", players[i].name, bet);
                players[i].amount -= bet;
                game->pot += bet;
                
            }
            if (!players[i].isComputer && players[i].isBetter){
                printf("What would you like to bet? (ex: 1, 10, 50), you have: %d\n", players[i].amount);
            bet:
                scanf("%s", result);
                bet = atoi(result);
                if (bet > players[i].amount || bet < 0){
                    printf("Too high or too low, please try again\n");
                    goto bet;
                }
                printf("%s bet %d\n", players[i].name, bet);
                players[i].amount -= bet;
                game->pot += bet;
            }
        }
        /* if player is not better, he must meet the bet or fold */
        for (i = 0; i < player_size; ++i) {
            if (!players[i].hasFolded && players[i].isActive){
                if (players[i].isComputer && !players[i].isBetter){
                    if (players[i].amount - bet < 0) {
                        players[i].hasFolded = 1;
                        printf("%s has folded\n", players[i].name);
                    }
                    else {
                        players[i].amount -= bet;
                        printf("%s called the bet\n", players[i].name);
                        game->pot += bet;
                    }
                }
                if (!players[i].isComputer && !players[i].isBetter){
                    if (players[i].amount - bet < 0) {
                        players[i].hasFolded = 1;
                        printf("%s has folded\n", players[i].name);
                    }
                    else {
                        printf("What would you like to do? Call or Fold? (ex: 'c' or 'f') \n");
                        scanf("%s", result);
                        if (!strncmp(result, "c", strlen("c"))){
                            players[i].amount -= bet;
                            printf("%s called the bet\n", players[i].name);
                            game->pot += bet;
                        }
                        else {
                            players[i].hasFolded = 1;
                            printf("%s has folded\n", players[i].name);
                        }
                    }
                }
            }
        }
        printf("Current pot at: %d\n", game->pot);
    }
    
    free(result);
}

