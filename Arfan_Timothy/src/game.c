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

