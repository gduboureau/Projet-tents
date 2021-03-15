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
#define ImGrass "grass.png"
#define ImEmpty "empty.png"
#define ImLosing "losing.png"
#define BACKGROUND "background.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *tree;
  SDL_Texture *tent;
  SDL_Texture *grass;
  SDL_Texture *empty;
  SDL_Texture *losing;
  SDL_Texture *text;
  SDL_Rect rect_t;
  SDL_Rect rect_l;
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

  PRINT("Help for commands :\n");
  PRINT("- Click the left mouse button to place a tent.\n");
  PRINT("- Click the right mouse button to place a grass.\n");
  PRINT("- Press or Scroll down the mouse wheel button to erase a square.\n");
  PRINT("- Press 'Z' on the keyboard to undo the last move.\n");
  PRINT("- Press 'Y' on the keyboard to redo the last move.\n");
  PRINT("- Press 'R' on the keyboard to restart the game.\n");
  PRINT("- Press 'S' on the keyboard to solve the game.\n");
  PRINT("- Press 'Q' on the keyboard to quit the game.\n");
  PRINT("Good luck and enjoy this game :) \n");

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) {
    ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  }

  /* init tree texture from PNG image */
  env->tree = IMG_LoadTexture(ren, ImTree);
  if (!env->tree) {
    ERROR("IMG_LoadTexture: %s\n", ImTree);
  }

  /* init tent texture from PNG image */
  env->tent = IMG_LoadTexture(ren, ImTent);
  if (!env->tent) {
    ERROR("IMG_LoadTexture: %s\n", ImTent);
  }

  /* init grass texture from PNG image */
  env->grass = IMG_LoadTexture(ren, ImGrass);
  if (!env->grass) {
    ERROR("IMG_LoadTexture: %s\n", ImGrass);
  }

  /* init empty texture from PNG image */
  env->empty = IMG_LoadTexture(ren, ImEmpty);
  if (!env->empty) {
    ERROR("IMG_LoadTexture: %s\n", ImEmpty);
  }

  /* init losing texture from PNG image */
  env->losing = IMG_LoadTexture(ren, ImLosing);
  if (!env->losing) {
    ERROR("IMG_LoadTexture: %s\n", ImLosing);
  }

  return env;
}

/* **************************************************************** */
void aux(SDL_Window *win, SDL_Renderer *ren, Env *env, uint i, uint j,
         SDL_Texture *text, square squares) {
  SDL_Rect rect;
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int w1 = (double)w / ((game_nb_cols(env->g)) + 2);
  int h1 = (double)h / ((game_nb_rows(env->g)) + 2);
  if (game_check_move(env->g, i, j, squares) == LOSING) {
    SDL_QueryTexture(env->losing, NULL, NULL, &env->rect_l.w, &env->rect_l.h);
    env->rect_l.w = w1 - 1;
    env->rect_l.h = h1 - 1;
    env->rect_l.x = w1 * j + w1 + w1 / 2 - env->rect_l.w / 2;
    env->rect_l.y = h1 * i + h1 + h1 / 2 - env->rect_l.h / 2;
    SDL_RenderCopy(ren, env->losing, NULL, &env->rect_l);
  }
  SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
  rect.w = w1;
  rect.h = h1;
  rect.x = w1 * j + w1 + w1 / 2 - rect.w / 2;
  rect.y = h1 * i + h1 + h1 / 2 - rect.h / 2;
  SDL_RenderCopy(ren, text, NULL, &rect);
}

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  int w1 = (double)w / ((game_nb_cols(env->g)) + 2);  // taille des cases
  int h1 = (double)h / ((game_nb_rows(env->g)) + 2);

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(ren, w1, h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w1, h1, w1, h - h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w - w1, h1);
  SDL_RenderDrawLine(ren, w - w1, h - h1, w1, h - h1);

  /* render text texture */
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
        aux(win, ren, env, i, j, env->tree, TREE);
      }
      if (game_get_square(env->g, i, j) == TENT) {
        aux(win, ren, env, i, j, env->tent, TENT);
      }
      if (game_get_square(env->g, i, j) == GRASS) {
        aux(win, ren, env, i, j, env->grass, GRASS);
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
  if (game_is_over(env->g)) {
    /* init text texture using Arial font */
    SDL_Color color_y = {0, 0, 0, 255}; /* blue color in RGBA */
    TTF_Font *fontt = TTF_OpenFont(FONT, 38);
    if (!fontt) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(fontt,
                     TTF_STYLE_BOLD);  // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
    SDL_Surface *surff = TTF_RenderText_Blended(
        fontt, "Congratulations ! You Win :)",
        color_y);  // blended rendering for ultra nice text
    env->text = SDL_CreateTextureFromSurface(ren, surff);
    SDL_FreeSurface(surff);
    TTF_CloseFont(fontt);
    /* render text texture */
    SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
    rect.x = w / 2 - rect.w / 2;
    rect.y = h / 2 - rect.h / 2;
    SDL_RenderCopy(ren, env->text, NULL, &rect);
  }
}

/* **************************************************************** */
void aux2(SDL_Window *win, SDL_Renderer *ren, Env *env, square squares,
          SDL_Texture *text) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int w1 = (double)w / (game_nb_cols(env->g) + 2);  // taille des cases
  int h1 = (double)h / (game_nb_rows(env->g) + 2);
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);
  if (mouse.x <= w - w1 && mouse.x >= w1 && mouse.y <= h - h1 &&
      mouse.y >= h1) {
    SDL_QueryTexture(text, NULL, NULL, &env->rect_t.w, &env->rect_t.h);
    env->rect_t.w = w1;
    env->rect_t.h = h1;
    uint i = (mouse.y + env->rect_t.h / 2 - h1 / 2 - h1) / h1;
    uint j = (mouse.x + env->rect_t.w / 2 - w1 / 2 - w1) / w1;
    if (game_check_move(env->g, i, j, squares) == REGULAR) {
      game_play_move(env->g, i, j, squares);
      env->rect_t.x = w1 * j + w1 + w1 / 2 - env->rect_t.w / 2;
      env->rect_t.y = h1 * i + h1 + h1 / 2 - env->rect_t.h / 2;
    }
    if (game_check_move(env->g, i, j, squares) == LOSING) {
      game_play_move(env->g, i, j, squares);
      SDL_QueryTexture(env->losing, NULL, NULL, &env->rect_l.w, &env->rect_l.h);
      env->rect_l.w = w1 - 1;
      env->rect_l.h = h1 - 1;
      uint i = (mouse.y + env->rect_l.h / 2 - h1 / 2 - h1) / h1;
      uint j = (mouse.x + env->rect_l.w / 2 - w1 / 2 - w1) / w1;
      env->rect_l.x = w1 * j + w1 + w1 / 2 - env->rect_l.w / 2;
      env->rect_l.y = h1 * i + h1 + h1 / 2 - env->rect_l.h / 2;
      env->rect_t.x = w1 * j + w1 + w1 / 2 - env->rect_t.w / 2;
      env->rect_t.y = h1 * i + h1 + h1 / 2 - env->rect_t.h / 2;
    }
  }
}

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
    aux2(win, ren, env, TENT, env->tent);
  }
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT) {
    aux2(win, ren, env, GRASS, env->grass);
  }
  if ((e->type == SDL_MOUSEWHEEL && e->wheel.y < 0) ||
      (e->type == SDL_MOUSEBUTTONDOWN &&
       e->button.button == SDL_BUTTON_MIDDLE)) {
    aux2(win, ren, env, EMPTY, env->empty);

  } else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_r:
        game_restart(env->g);
        break;
      case SDLK_z:
        game_undo(env->g);
        break;
      case SDLK_y:
        game_redo(env->g);
        break;
      case SDLK_s:
        game_solve(env->g);
        break;
      case SDLK_q:
        return true;
    }
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