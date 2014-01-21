#include "model.h"

const int SCREEN_WIDTH = 952;
const int SCREEN_HEIGHT = 631;

int pNum; // player number

RISK_move move_queue[10]; // NULL terminated

// logs any SDL error, taking op as the operation that failed
void log_SDL_error(const char *op);

// initialize SDL
int init_SDL();

// cleanup SDL
void cleanup_SDL();

// load texture
SDL_Texture *load_texture(const char*);

// surface to texture
SDL_Texture *surftotex(SDL_Surface*);
