#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "model.h"

const int SCREEN_WIDTH = 952;
const int SCREEN_HEIGHT = 631;

// logs any SDL error, taking op as the operation that failed
void log_SDL_error(const char *op);

// initialize SDL
int init_SDL();

// cleanup SDL
void cleanup_SDL();

// load texture
SDL_Texture *load_texture(const char*);
