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
#define FONTSIZE 16
#define ImTree "tree.png"
#define ImTent "tent.png"
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
  SDL_Rect rect_t;
  game g;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc == 2) {
    env->g = game_load(argv[1]);
  } else {
    env->g = game_default();
  }
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) {
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  }

  /* init tree texture from PNG image */
  env->tree = IMG_LoadTexture(ren, ImTree);

  /* init tent texture from PNG image */
  env->tent = IMG_LoadTexture(ren, ImTent);

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

  int w1 = w / ((game_nb_cols(env->g)) + 2);  // taille des cases
  int h1 = h / ((game_nb_rows(env->g)) + 2);

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(ren, w1, h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w1, h1, w1, h - h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w1, h - h1);

  /* render tree texture */
  for (uint i = 0; i < game_nb_rows(env->g); i++) {
    SDL_Color color = {0, 0, 0, 0}; /* black color in RGBA */
    TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE * (w / (double)SCREEN_WIDTH));
    if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    char chaine[game_nb_rows(env->g)];
    sprintf(chaine, "%d", i);
    SDL_Surface *surf = TTF_RenderText_Blended(font, chaine, color);
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);

    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    rect.x = w1 / 2 - rect.w / 2;
    rect.y = i * h1 + h1 + h1 / 2 - rect.h / 2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);

    sprintf(chaine, "%d", game_get_expected_nb_tents_row(env->g, i));
    SDL_Surface *surf2 = TTF_RenderText_Blended(font, chaine, color);
    env->text = SDL_CreateTextureFromSurface(ren, surf2);
    SDL_FreeSurface(surf2);
    TTF_CloseFont(font);

    rect.x = w - w1 / 2 - rect.w / 2;
    rect.y = i * h1 + h1 + h1 / 2 - rect.h / 2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
  }
  for (uint j = 0; j < game_nb_cols(env->g); j++) {
    /* render text texture */
    SDL_Color color = {0, 0, 0, 0}; /* black color in RGBA */
    TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE * (h / (double)SCREEN_HEIGHT));
    if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    char chaine[game_nb_cols(env->g)];
    sprintf(chaine, "%d", j);
    SDL_Surface *surf = TTF_RenderText_Blended(font, chaine, color);
    env->text = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);

    rect.x = j * w1 + w1 + w1 / 2 - rect.w / 2;
    rect.y = h1 / 2 - rect.h / 2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);

    sprintf(chaine, "%d", game_get_expected_nb_tents_col(env->g, j));
    SDL_Surface *surf3 = TTF_RenderText_Blended(font, chaine, color);
    env->text = SDL_CreateTextureFromSurface(ren, surf3);
    SDL_FreeSurface(surf3);
    TTF_CloseFont(font);

    rect.x = j * w1 + w1 + w1 / 2 - rect.w / 2;
    rect.y = h - h1 / 2 - rect.h / 2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
  }
  for (uint i = 0; i < game_nb_rows(env->g); i++) {
    for (uint j = 0; j < game_nb_cols(env->g); j++) {
      /* render tree texture */
      if (game_get_square(env->g, i, j) == TREE) {
        SDL_QueryTexture(env->tree, NULL, NULL, &rect.w, &rect.h);
        rect.w = (w / (double)SCREEN_WIDTH) * rect.w *
                 ((double)DEFAULT_SIZE / game_nb_rows(env->g));
        rect.h = (h / (double)SCREEN_HEIGHT) * rect.h *
                 ((double)DEFAULT_SIZE / game_nb_cols(env->g));
        rect.x = w1 * j + w1 + w1 / 2 - rect.w / 2;
        rect.y = h1 * i + h1 + h1 / 2 - rect.h / 2;
        SDL_RenderCopy(ren, env->tree, NULL, &rect);
      }
      if (game_get_square(env->g, i, j) == TENT) {
        SDL_QueryTexture(env->tent, NULL, NULL, &rect.w, &rect.h);
        rect.w = (w / (double)SCREEN_WIDTH) * rect.w *
                 ((double)DEFAULT_SIZE / game_nb_rows(env->g));
        rect.h = (h / (double)SCREEN_HEIGHT) * rect.h *
                 ((double)DEFAULT_SIZE / game_nb_cols(env->g));
        rect.x = w1 * j + w1 + w1 / 2 - rect.w / 2;
        rect.y = h1 * i + h1 + h1 / 2 - rect.h / 2;
        SDL_RenderCopy(ren, env->tent, NULL, &rect);
      }
      if (game_get_square(env->g, i, j) == GRASS) {
        SDL_QueryTexture(env->grass, NULL, NULL, &rect.w, &rect.h);
        rect.w = (w / (double)SCREEN_WIDTH) * rect.w *
                 ((double)DEFAULT_SIZE / game_nb_rows(env->g));
        rect.h = (h / (double)SCREEN_HEIGHT) * rect.h *
                 ((double)DEFAULT_SIZE / game_nb_cols(env->g));
        rect.x = w1 * j + w1 + w1 / 2 - rect.w / 2;
        rect.y = h1 * i + h1 + h1 / 2 - rect.h / 2;
        SDL_RenderCopy(ren, env->grass, NULL, &rect);
      }
    }
  }

  if (game_nb_cols(env->g) > game_nb_rows(env->g)) {
    for (uint i = 0; i < game_nb_cols(env->g) - 1; i++) {
      SDL_RenderDrawLine(ren, w1 * 2 + i * w1, h1, w1 * 2 + i * w1, h - h1);
      SDL_RenderDrawLine(ren, w1, h1 * 2 + i * h1, w - w1, h1 * 2 + i * h1);
    }
  } else {
    for (uint i = 0; i < game_nb_rows(env->g) - 1; i++) {
      SDL_RenderDrawLine(ren, w1 * 2 + i * w1, h1, w1 * 2 + i * w1, h - h1);
      SDL_RenderDrawLine(ren, w1, h1 * 2 + i * h1, w - w1, h1 * 2 + i * h1);
    }
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int w1 = w / ((game_nb_cols(env->g)) + 2);  // taille des cases
  int h1 = h / ((game_nb_rows(env->g)) + 2);

  if (e->type == SDL_QUIT) {
    return true;
  }

  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (mouse.x <= w - w1 && mouse.x >= w1 && mouse.y <= h - h1 &&
        mouse.y >= h1) {
      SDL_QueryTexture(env->tent, NULL, NULL, &env->rect_t.w, &env->rect_t.h);
      env->rect_t.w = (w / (double)SCREEN_WIDTH) * env->rect_t.w *
                      ((double)DEFAULT_SIZE / game_nb_rows(env->g));
      env->rect_t.h = (h / (double)SCREEN_HEIGHT) * env->rect_t.h *
                      ((double)DEFAULT_SIZE / game_nb_cols(env->g));
      uint i = (mouse.y + env->rect_t.h / 2 - h1 / 2 - h1) / h1;
      uint j = (mouse.x + env->rect_t.w / 2 - w1 / 2 - w1) / w1;
      if (game_check_move(env->g, i, j, TENT) == REGULAR) {
        game_play_move(env->g, i, j, TENT);
        env->rect_t.x = w1 * j + w1 + w1 / 2 - env->rect_t.w / 2;
        env->rect_t.y = h1 * i + h1 + h1 / 2 - env->rect_t.h / 2;
      }
    }
  }

  if (game_is_over(env->g)) {
    return true;
  }

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
  game_delete(env->g);
  free(env);
}

/* **************************************************************** */