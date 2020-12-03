#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

#define DEFAULT_SIZE 8

typedef struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
} game_s;

typedef struct game_s *game;

typedef const struct game_s *cgame;

/********************* Jennifer *********************/

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  return 0;
}

game game_new_empty(void) {
  game g = malloc(sizeof(game));
  if (g == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  g->squares = malloc(sizeof(square) * DEFAULT_SIZE * DEFAULT_SIZE);
  g->nb_tents_row = malloc(sizeof(uint) * DEFAULT_SIZE);
  g->nb_tents_col = malloc(sizeof(uint) * DEFAULT_SIZE);
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tents_row[i] = 0;
    g->nb_tents_col[i] = 0;
  }
  for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
    g->squares[j] = EMPTY;
  }
  return g;
}

game game_copy(cgame g) { return 0; }

bool game_equal(cgame g1, cgame g2) { 
  for(uint i=0;i<DEFAULT_SIZE;i++){
    for(uint j=0;j<DEFAULT_SIZE;j++){
      if(game_get_square(g1,i,j) == game_get_square(g2,i,j) && game_get_expected_nb_tents_row(g1,i) == game_get_expected_nb_tents_row(g2,i) && game_get_expected_nb_tents_col(g1,j) == game_get_expected_nb_tents_col(g2,j)){
        return true;
      }
    }
  }
  return false; }

void game_delete(game g) {
  if (g == NULL) {
    return ;
  }
  free(g->squares);
  g->squares = NULL;
  free(g->nb_tents_row);
  g->nb_tents_row = NULL;
  free(g->nb_tents_col);
  g->nb_tents_col = NULL;
  free(g);
  g = NULL;
}

void game_set_square(game g, uint i, uint j, square s) { return; }

/********************* Hugo *********************/

square game_get_square(cgame g, uint i, uint j) {
  if (g == NULL || i >= DEFAULT_SIZE || j >= DEFAULT_SIZE || i < 0 || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->squares[(i * DEFAULT_SIZE) + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  if (g == NULL || i >= DEFAULT_SIZE || i < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_row[i] = nb_tents;
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  if (g == NULL || j >= DEFAULT_SIZE || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_col[j] = nb_tents;
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  if (g == NULL || i >= DEFAULT_SIZE || i < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_tents_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= DEFAULT_SIZE || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_tents_col[j];
}

/********************* Valentin *********************/

uint game_get_expected_nb_tents_all(cgame g) {
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    cpt += game_get_expected_nb_tents_row(g, j);
  }
  return cpt;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  if (g == NULL || i >= DEFAULT_SIZE) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k; k < DEFAULT_SIZE; k++) {
    if (game_get_square(g, i, k) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= DEFAULT_SIZE) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k; k < DEFAULT_SIZE; k++) {
    if (game_get_square(g, k, j) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_all(cgame g) {
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    cpt += game_get_current_nb_tents_row(g, j);
  }
  return cpt;
}

void game_play_move(game g, uint i, uint j, square s) { return; }

/********************* Guillaume *********************/

int game_check_move(cgame g, uint i, uint j, square s) { return 0; }

bool game_is_over(cgame g) { return 0; }

void game_fill_grass_row(game g, uint i) { return; }

void game_fill_grass_col(game g, uint j) { return; }

void game_restart(game g) { return; }