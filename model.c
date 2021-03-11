#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

#define ImTree "tree.png"
#define BACKGROUND "background.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture* background;
  SDL_Texture* tree;
  SDL_Texture* tent;
  SDL_Texture* grass;
  SDL_Texture* empty;
  SDL_Texture* text;
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
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  if(argc==1){
    game g = game_default();
    w = SCREEN_WIDTH;
    h = SCREEN_HEIGHT;
    int r = game_nb_rows(g);
    int c = game_nb_cols(g);
    int w1=w/(c+2); // 60 pour DS
    int h1=h/(r+2); // 60 pour DS
    SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(ren, w1, h1, w-w1, h1);
    SDL_RenderDrawLine(ren, w1, h1, w1, h-h1);
    SDL_RenderDrawLine(ren, w-w1, h-h1, w-w1, h1);
    SDL_RenderDrawLine(ren, w-w1, h-h1, w1, h-h1);
    for(uint i; i<game_nb_rows(g); i++){
        SDL_RenderDrawLine(ren, w1*2+i*w1, h1, w1*2+i*w1, h-h1);
        SDL_RenderDrawLine(ren, w1, h1*2+i*h1, w-w1, h1*2+i*h1);
    }
    for(uint i; i<game_nb_rows(g); i++){
      for(uint j; j<game_nb_cols(g); j++){
        if(game_get_square(g,i,j)==TREE){
          env->tree_x = 60*j+60;
          env->tree_y = 60*i+60;
          env->tree = IMG_LoadTexture(ren, ImTree);
          if (!env->tree) ERROR("IMG_LoadTexture: %s\n", ImTree);
        }
      }
    }
  }
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren,
            Env *env) {
  SDL_Rect rect;
  
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */


  /* render tree texture */
  SDL_QueryTexture(env->tree, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->tree_x - rect.w / 2;
  rect.y = env->tree_y - rect.h / 2;
  SDL_RenderCopy(ren, env->tree, NULL, &rect);
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