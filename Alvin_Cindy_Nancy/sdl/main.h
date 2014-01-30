#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void loadBMPs(char *file, SDL_Surface *dest);
void errorMessage(char *error);
void init();
void draw(SDL_Surface *screen, SDL_Surface *img, int x, int y);

