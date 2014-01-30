#include "main.h"
#include "../maze.h"

int main(int argc, char *argv[]){
  SDL_Surface *screen;
  SDL_Surface *wall;
  SDL_Surface *floor;
  SDL_Surface *minion;
  SDL_Event event;
  
  init();
  screen = SDL_SetVideoMode(100, 100, 1, SDL_SWSURFACE | SDL_ANYFORMAT);
  if (screen == NULL){
    errorMessage("Could not set up video.");
    exit(0);
  }
  
  char *maze = (char *) malloc(100 * sizeof(char));
  GenerateMaze(maze, 23, 23);
  
  int x = 0;
  int y = 0;
  int cx = 1;
  int cy = 1;
  int done = 0;
  
  loadBMPs("Wall.bmp", wall);
  loadBMPs("Floor.bmp", floor);
  loadBMPs("NotMeatBoy.bmp", minion);
  
  int row, col;
  while (!done){
    for (row = 0; row < 10; row ++){
      for (col = 0; col < 10; col++){
	if (maze[row*10 +col] == 1)
	  draw(screen, wall, col, row);
	else
	  draw(screen, floor, col, row);
      }
    }
  }
  draw(screen, minion, cx, cy);

  while (SDL_PollEvent(&event)){
    switch (event.type){
    case SDL_QUIT:
      done = 1;
      break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym){
      case SDLK_LEFT:
	if (maze[cy*10+(cx-1)] == 0){
	  cx = cx -1;
	  break;
	}
	else
	  break;
      case SDLK_RIGHT:
	if (maze[cy*10+(cx+1)] == 0){
	  cx = cx + 1;
	  break;
	}
	else
	  break;
      case SDLK_UP:
	if (maze[(cy-1)*10 +cx] == 0){
	  cy = cy -1;
	  break;
	}
	else
	  break;
      case SDLK_DOWN:
	if (maze[(cy+1)*10+cy] == 0){
	  cy = cy + 1;
	  break;
	}
	else
	  break;
      default: //incase player presses random key.
	break;
      }
    }
  }
  
  SDL_FreeSurface(wall);
  SDL_FreeSurface(floor);
  SDL_FreeSurface(minion);
  SDL_FreeSurface(screen);
  SDL_Quit();

  return 0;
}
  

void loadBMPs(char *file, SDL_Surface *dest){
  SDL_Surface *temp = SDL_LoadBMP(file);
  if (temp == NULL){
    errorMessage("Cannot load images.");
    SDL_Quit();
    exit(0);
  }
  dest = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
}

void errorMessage(char *error){
  printf("%s\n", error);
}

void init(){
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
    errorMessage("Could not initiate.");
    exit(0);
  }
}

void draw(SDL_Surface *screen, SDL_Surface *img, int ax, int ay){
  SDL_Rect *r;
  r.x = ax;
  r.y = ay;
  SDL_BlitSurface(img, NULL, screen, r);
}
