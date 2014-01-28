#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define MAX_MAP_X 20
#define MAX_MAP_Y 15
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define TILE_SIZE 32

enum
  {
    PLAYER_SPRITE,
    MAX_SPRITES
  };
