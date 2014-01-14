#include "ui.h"

SDL_Window *win;
SDL_Renderer *ren;

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
  SDL_Texture *maptex = load_texture("maptex.bmp");
  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, maptex, NULL, NULL);
  SDL_RenderPresent(ren);
  SDL_DestroyTexture(maptex);
  return 0;
}

void cleanup_SDL() {
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

/*
void draw_circle(circle c) {

}*/

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

  while (!done) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
	done = 1;
      if (event.type == SDL_MOUSEBUTTONDOWN) {
	printf("mouse down at (%d, %d)\n", event.button.x, event.button.y);
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 100);
	if (!SDL_RenderDrawPoint(ren, event.button.x, event.button.y))
	  log_SDL_error("draw rect");
	SDL_RenderPresent(ren);
      }
    }
  }
  cleanup_SDL();
}
