#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "ui.h"
#include "map.h"

SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *maptex;

struct color {int r, g, b;};
struct color pColors[] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};

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
  //SDL_RenderPresent(ren);
  return 0;
}

void cleanup_SDL() {
  SDL_DestroyTexture(maptex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}


SDL_Texture *circle_texture(int r, int g, int b) {
  printf("loading circle\n");
  SDL_Surface *img = SDL_LoadBMP("circle.bmp");

  SDL_SetColorKey(img, SDL_TRUE, 
		  SDL_MapRGB(img->format, 0, 0, 0));
  SDL_Texture *tex = surftotex(img);
  if (SDL_SetTextureColorMod(tex, r, g, b) != 0)
    log_SDL_error("modifying circle color");
  return tex;
}


void draw_terr(territory t) {
  SDL_Rect dest_pos = {t.x, t.y, 40, 40};
  struct color pc = pColors[t.owner-1];
  SDL_Texture *circle = circle_texture(pc.r, pc.g, pc.b);
  SDL_RenderCopy(ren, circle, NULL, &dest_pos);
  // add text here
}

SDL_Texture *surftotex(SDL_Surface *img) {
  SDL_Texture *tex;
  if (img) {
    tex = SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);
    if (!tex)
      log_SDL_error("Texture from surface");
  } else
    log_SDL_error("SDL BMP loading");
  return tex;
}


SDL_Texture *load_texture(const char *path) {
  SDL_Surface *img = SDL_LoadBMP(path);
  return surftotex(img);
}


void update_map() {
  RISK_move *p;
  for (p=move_queue; p; p++) {
    if (!p->origin) {
      // a player is resupplying
      p->destination->units += p->units;
    } else if (p->origin->owner != p->destination->owner) {
      // destination lost a battle
      p->destination->units = p->units;
      p->destination->owner = p->origin->owner;
      p->origin->units -= p->units;
    } else if (p->origin->owner == p->destination->owner) {
      // troop movements
      p->destination->units += p->units;
      p->origin->units -= p->units;
    }
    SDL_Texture *circ = circle_texture(255, 0, 0);
    SDL_RenderCopy(ren, circ, NULL, NULL);
  }
}

int main() {
  if (init_SDL())
    return 1;

  SDL_Event event;
  char done = 0;
  terrs = territories();
  int i = 0;
  while(terrs[i].name) {
    printf("drawing %s at (%d, %d)\n", terrs[i].name,
           terrs[i].x, terrs[i].y);
#warning This is a test
    terrs[i].owner = 1;
    draw_terr(terrs[i]);
    i++;
  }
  printf("done drawing terrs\n");
  SDL_RenderPresent(ren);
  while (!done) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        done = 1;
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        printf("mouse down at (%d, %d)\n", event.button.x, event.button.y);

      }
      /* SDL_RenderClear(ren); */
      /* SDL_RenderCopy(ren, maptex, NULL, NULL); */
      //SDL_Surface *surface =
    }
  }
  cleanup_SDL();
}
