#include "model.h"

const int SCREEN_WIDTH = 952;
const int SCREEN_HEIGHT = 631;

int pNum; // player number

RISK_move *move_queue; // NULL terminated

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

// get a circle texture
SDL_Texture *circle_texture(int r, int g, int b);

// draw a territory
void draw_terr(territory t);

// check if a territory contains a click
territory *terr_click(SDL_MouseButtonEvent m);

// walks the move queue and executes all moves to be made
void update_map();

// opens TTF_Font
TTF_Font *open_font();

// renders text to a texture
SDL_Texture *text_texture(char *);

// queues a move to be processed next update
void queue_move(RISK_move m);
