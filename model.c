#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

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
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc == 2){
    game g = game_load(argv[1]);
    env->nb_rows = game_nb_rows(g);
    env->nb_cols = game_nb_cols(g);
    env->nb_tents_row = malloc(sizeof(uint) * env->nb_rows);
    for (uint i = 0; i < game_nb_rows(g); i++) {
      env->nb_tents_row[i] = game_get_expected_nb_tents_row(g,i);
    }
    env->nb_tents_col = malloc(sizeof(uint) * env->nb_cols);
    for (uint i = 0; i < game_nb_cols(g); i++) {
      env->nb_tents_col[i] = game_get_expected_nb_tents_col(g,i);
    }
    env->squares = malloc(sizeof(uint) * (env->nb_cols)* (env->nb_rows));
    for (uint i = 0; i < game_nb_rows(g); i++) {
      for (uint j = 0; j < game_nb_cols(g); j++){
      env->squares[(i * env->nb_cols + j)] = game_get_square(g,i,j);
      }
    }
    game_delete(g);
  }else{
    game g = game_default();
    env->nb_rows = game_nb_rows(g);
    env->nb_cols = game_nb_cols(g);
    env->nb_tents_row = malloc(sizeof(uint) * env->nb_rows);
    for (uint i = 0; i < game_nb_rows(g); i++) {
      env->nb_tents_row[i] = game_get_expected_nb_tents_row(g,i);
    }
    env->nb_tents_col = malloc(sizeof(uint) * env->nb_cols);
    for (uint i = 0; i < game_nb_cols(g); i++) {
      env->nb_tents_col[i] = game_get_expected_nb_tents_col(g,i);
    }
    env->squares = malloc(sizeof(uint) * (env->nb_cols)* (env->nb_rows));
    for (uint i = 0; i < game_nb_rows(g); i++) {
      for (uint j = 0; j < game_nb_cols(g); j++){
      env->squares[(i * env->nb_cols + j)] = game_get_square(g,i,j);
      }
    }
    game_delete(g);
  }
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) {
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  }
  
  /* init tree texture from PNG image */
  env->tree = IMG_LoadTexture(ren, ImTree);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  int w1 = w / ((env->nb_rows)+2);  // taille des cases
  int h1 = h / ((env->nb_cols)+2);

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(ren, w1, h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w1, h1, w1, h - h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w1, h - h1);

  /* render tree texture */
  for (uint i = 0; i < env->nb_rows; i++) {
    for (uint j = 0; j < env->nb_cols; j++) {
      /* render text texture */
      SDL_Color color = {0, 0, 0, 0}; /* black color in RGBA */
      TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
      if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
      TTF_SetFontStyle(font, TTF_STYLE_BOLD);
      char chaine[16];
      sprintf(chaine, "%d", i); 
      SDL_Surface *surf = TTF_RenderText_Blended(font, chaine, color);
      env->text = SDL_CreateTextureFromSurface(ren, surf);
      SDL_FreeSurface(surf);
      TTF_CloseFont(font);

      SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
      rect.x = w1/2 - rect.w / 2;
      rect.y = i * h1 + h1+h1/2 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = i * w1 + w1+w1/2 - rect.w / 2;
      rect.y = h1/2 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = w-w1/2 - rect.w / 2;
      rect.y = i * h1 + h1+h1/2 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);

      rect.x = i * w1 + w1+w1/2 - rect.w / 2;
      rect.y = h-h1/2 - rect.h / 2;
      SDL_RenderCopy(ren, env->text, NULL, &rect);
      /* render tree texture */
      if (env->squares[(i * env->nb_cols + j)] == TREE) {
        SDL_QueryTexture(env->tree, NULL, NULL, &rect.w, &rect.h);
        rect.x = w1 * j + w1+w1/2 - rect.w / 2;
        rect.y = h1 * i + h1+h1/2 - rect.h / 2;
        SDL_RenderCopy(ren, env->tree, NULL, &rect);
      }
    }
  }

  for (uint i = 0; i < env->nb_rows-1; i++) {
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