#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

#include "model.h"

#define FONT "arial.ttf"
#define FONTSIZE 36
#define ImTree "tree.png"
#define BACKGROUND "background.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *tree;
  SDL_Texture *tent;
  SDL_Texture *grass;
  SDL_Texture *empty;
  SDL_Texture *text;
  int tent_x, tent_y;
  int grass_x, grass_y;
  int empty_x, empty_y;
  int tree_x, tree_y;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) {
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  }

  /* init text texture using Arial font */
      SDL_Color color = {0, 0, 0, 0}; /* black color in RGBA */
      TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
      if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
      TTF_SetFontStyle(font, TTF_STYLE_BOLD);
      SDL_Surface *surf = TTF_RenderText_Blended(font, "0", color);
      env->text = SDL_CreateTextureFromSurface(ren, surf);
      SDL_FreeSurface(surf);
      TTF_CloseFont(font);
  
  /* init tree texture from PNG image */
  env->tree = IMG_LoadTexture(ren, ImTree);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;
  game g = game_default();

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  int w1 = w / 10;  // 60 pour DS
  int h1 = h / 10;  // 60 pour DS

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(ren, w1, h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w1, h1, w1, h - h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w1, h - h1);

  /* render tree texture */
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      /* render text texture */
      SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
      rect.x = 30 - rect.w / 2;
      rect.y = i * 60 + 90 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = i * 60 + 90 - rect.w / 2;
      rect.y = 30 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = 570 - rect.w / 2;
      rect.y = i * 60 + 90 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = i * 60 + 90 - rect.w / 2;
      rect.y = 570 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      if (game_get_square(g, i, j) == TREE) {
        SDL_QueryTexture(env->tree, NULL, NULL, &rect.w, &rect.h);
        rect.x = 60 * j + 90 - rect.w / 2;
        rect.y = 60 * i + 90 - rect.h / 2;
        SDL_RenderCopy(ren, env->tree, NULL, &rect);
      }
    }
  }

  for (uint i = 0; i < game_nb_rows(g); i++) {
    SDL_RenderDrawLine(ren, w1 * 2 + i * w1, h1, w1 * 2 + i * w1, h - h1);
    SDL_RenderDrawLine(ren, w1, h1 * 2 + i * h1, w - w1, h1 * 2 + i * h1);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->tree);
  SDL_DestroyTexture(env->tent);
  SDL_DestroyTexture(env->grass);
  SDL_DestroyTexture(env->empty);
  SDL_DestroyTexture(env->text);
  free(env);
}

/* **************************************************************** */