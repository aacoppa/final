#include "ui.h"

SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *maptex;


void log_SDL_error(const char *op) {
  printf("%s failed:\n\t%s\n", op, SDL_GetError());
}

int init_SDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    log_SDL_error("SDL init");
    return 1;
  }
  win = SDL_CreateWindow("Map", 100, 100, SCREEN_WIDTH,
				     SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!win) {
    log_SDL_error("SDL window create");
    return 1;
  }
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED 
			   | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) {
    log_SDL_error("SDL renderer creation");
    return 1;
  }
  maptex = load_texture("maptex.bmp");
  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, maptex, NULL, NULL);
  SDL_RenderPresent(ren);
  return 0;
}

void cleanup_SDL() {
  SDL_DestroyTexture(maptex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}


SDL_Texture *circle_texture(int r, int g, int b) {
  SDL_Texture *tex = load_texture("circle.bmp");
  if (SDL_SetTextureColorMod(tex, r, g, b) != 0)
    log_SDL_error("modifying circle color");
  return tex;
}


void draw_terr(territory t, SDL_Surface *s) {
  // lookup t coordinates here
  int x, y;
  SDL_Rect dest_pos = {x, y, 40, 40};
  // lookup player color here v
  int r, g, b;
  SDL_Texture *circle = circle_texture(r, g, b);
  SDL_RenderCopy(ren, circle, NULL, &dest_pos);
  // add text here
}

SDL_Texture *load_texture(const char *path) {
  SDL_Texture *tex = NULL;
  SDL_Surface *img = SDL_LoadBMP(path);
  if (img) {
    tex = SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);
    if (!tex)
      log_SDL_error("Texture from surface");
  } else
    log_SDL_error("SDL BMP loading");
  return tex;
}


int main() {
  if (init_SDL())
    return 1;

  SDL_Event event;
  char done = 0;
  // Not working
  SDL_Texture *circ = circle_texture(255, 0, 0);
  SDL_RenderCopy(ren, circ, NULL, NULL);
  while (!done) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
	done = 1;
      if (event.type == SDL_MOUSEBUTTONDOWN) {
	printf("mouse down at (%d, %d)\n", event.button.x, event.button.y);
	//	mark_location(event.button.x, event.button.y);
      }
      SDL_RenderClear(ren);
      SDL_RenderCopy(ren, maptex, NULL, NULL);
      //SDL_Surface *surface = 
    }
  }
  cleanup_SDL();
}
