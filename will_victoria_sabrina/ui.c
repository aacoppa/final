#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "map.h"
#include "logic.h"
#include <math.h>
#include <assert.h>

SDL_Window *win;
SDL_Renderer *ren;
SDL_Texture *maptex;
TTF_Font *font = NULL;
territory *selected = NULL;
char done = 0;

SDL_Color pColors[] = {{255, 11, 111}, {15, 130, 255}, {255, 145, 15},
  {157, 73, 255}, {16, 204, 167}};

const int TERR_RADIUS = 20;

void log_SDL_error(const char *op) {
  printf("%s failed:\n\t%s\n", op, SDL_GetError());
}

int init_SDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    log_SDL_error("SDL init");
    return 1;
  }
  if (TTF_Init() != 0){
    log_SDL_error("TTF init");
    return 1;
  }
  win = SDL_CreateWindow("Map", 100, 100, SCREEN_WIDTH,
                         SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!win) {
    log_SDL_error("SDL window create");
    return 1;
  }
  ren = SDL_CreateRenderer(win, -1,  0);//SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) {
    log_SDL_error("SDL renderer creation");
    return 1;
  }
  // max 10 moves queued
  move_queue = calloc(sizeof(RISK_move), 10);
  maptex = load_texture("maptex.bmp");
  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, maptex, NULL, NULL);
  return 0;
}


SDL_Texture *circle_texture(int r, int g, int b) {
  SDL_Surface *img = SDL_LoadBMP("circle.bmp");
  
  SDL_SetColorKey(img, SDL_TRUE,
                  SDL_MapRGB(img->format, 0, 0, 0));
  SDL_Texture *tex = surftotex(img);
  if (SDL_SetTextureColorMod(tex, r, g, b) != 0)
    log_SDL_error("modifying circle color");
  return tex;
}

void draw_terr(territory t) {
  SDL_Rect dest_pos = {t.x, t.y, TERR_RADIUS*2, TERR_RADIUS*2};
  SDL_Color pc = pColors[t.owner-1];
  SDL_Texture *circle = circle_texture(pc.r, pc.g, pc.b);
  SDL_RenderCopy(ren, circle, NULL, &dest_pos);
  // allow up to 3 bytes
  char *tbuf = malloc(4 * sizeof(char));
  sprintf(tbuf, "%d", t.units);
  SDL_Texture *text = text_texture(tbuf);
  SDL_RenderCopy(ren, text, NULL, &dest_pos);
  // add text here
  SDL_DestroyTexture(circle);
  SDL_DestroyTexture(text);
}

territory *terr_click(SDL_MouseButtonEvent m) {
  int i = 0;
  while (terrs[i].name) {
    float d = hypot(terrs[i].x + TERR_RADIUS - m.x, terrs[i].y + TERR_RADIUS - m.y);
    // printf("checking %s ... %f\n", terrs[i].name, d);
    if (d <= TERR_RADIUS)
      return terrs+i;
    i++;
  }
  return NULL;
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

void queue_move(RISK_move m) {
  int i = 0;
  RISK_move *p = move_queue;
  while (p->units > 0) {
    i++;
    p++;
  }
  assert(i < 10);
}

void update_map() {
  RISK_move *p;
  char represent = 0;
  if (move_queue) {
    SDL_RenderClear(ren);
    represent = 1;
  }
  for (p=move_queue; p->units > 0; p++) {
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
    *p = (RISK_move){-1, NULL, NULL};
  }
  if (represent) {
    SDL_RenderCopy(ren, maptex, NULL, NULL);
    int i = 0;
    while(terrs[i].name) {
      draw_terr(terrs[i]);
      i++;
    }
    SDL_RenderPresent(ren);
  }
}

TTF_Font *open_font() {
  //Open the font
  TTF_Font *font = TTF_OpenFont("source-code-pro.ttf", 50);
  if (!font){
    log_SDL_error("font opening");
    return NULL;
  }
  return font;
}

SDL_Texture *text_texture(char *message) {
  if (!font)
    font = open_font();
  SDL_Color c = {0, 0, 0, 255};
  SDL_Surface *surf = TTF_RenderText_Blended(font, message, c);
  if (!surf){
    TTF_CloseFont(font);
    log_SDL_error("text_texture()");
    return NULL;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
  if (!texture){
    log_SDL_error("create text texture");
  }
  //Clean up the surface
  SDL_FreeSurface(surf);
  return texture;
}

void handle_input() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      printf("exiting RISK...\n");
      done = 1;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      territory *c = terr_click(event.button);
      if (c) {
        printf("clicked: ");
        log_terr(*c);
        if (selected) {
          //process();
          printf("%s to %s\n", selected->name, c->name);
          if (selected == c)
            printf("double clicked!\n");
          else {
            printf("territories are %sadjacent\n", tadjacent(selected, c)?"":"not ");
            selected = NULL;
          }
        } else {
          selected = c;
        }
      } else {
        printf("clicked (%d, %d)\n", event.button.x, event.button.y);
        selected = NULL;
      }
    }
  }
}

int main() {
  if (init_SDL())
    return 1;
  terrs = territories();
  distribute(5);
  int i = 0;
  while(terrs[i].name) {
    draw_terr(terrs[i]);
    i++;
  }
  printf("done drawing terrs\n");
  SDL_RenderPresent(ren);
  while (!done) {
    handle_input();
    update_map();
    SDL_Delay(100);
  }
  cleanup_SDL();
}

void cleanup_SDL() {
  TTF_CloseFont(font);
  SDL_DestroyTexture(maptex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}
