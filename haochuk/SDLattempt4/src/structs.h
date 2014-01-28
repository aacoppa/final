#include "defs.h"

typedef struct Game
{
  SDL_Surface *screen;
} Game;

typedef struct Map
{
	int tile[MAX_MAP_Y][MAX_MAP_X];
} Map;

typedef struct Sprites
{
  SDL_Surface *image;
} Sprites;

typedef struct Entity
{
  int w, h;
  int thinkTime;
  SDL_Surface *sprite;
  float x, y;
} Entity;

typedef struct Control
{
  int up, down, left, right;
} Control;
