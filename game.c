#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_ext.h"

typedef unsigned int uint;

#define DEFAULT_SIZE 8

typedef struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
  bool wrapping;
  bool diagadj;
} game_s;

typedef struct game_s *game;

typedef const struct game_s *cgame;

/********game_ext********/

/********************* Hugo *********************/

game game_new_ext(uint nb_rows, uint nb_cols, square *squares,
                  uint *nb_tents_row, uint *nb_tents_col, bool wrapping,
                  bool diagadj) {
  if (squares == NULL || nb_tents_row == NULL || nb_tents_col == NULL) {
    exit(EXIT_FAILURE);
  }
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, diagadj);
  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint i = 0; i < nb_cols; i++) {
    g->nb_tents_col[i] = nb_tents_col[i];
  }
  for (uint j = 0; j < nb_rows * nb_cols; j++) {
    g->squares[j] = squares[j];
  }
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;
  return g;
}
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool diagadj) {
  game g = malloc(sizeof(game_s));
  if (g == NULL) {
    fprintf(stderr, "Not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;
  g->nb_tents_row = malloc(sizeof(uint) * nb_rows);
  if (g->nb_tents_row == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_col = malloc(sizeof(uint) * nb_cols);
  if (g->nb_tents_col == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint i = 0; i < nb_cols; i++) {
    g->nb_tents_col[i] = 0;
  }

  g->squares = malloc(sizeof(square) * nb_rows * nb_cols);
  if (g->squares == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  for (uint j = 0; j < nb_rows * nb_cols; j++) {
    g->squares[j] = EMPTY;
  }

  return g;
}

/********************* Guillaume *********************/

uint game_nb_rows(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_rows;
}

uint game_nb_cols(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_cols;
}

/********************* Valentin *********************/

bool game_is_wrapping(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->wrapping;
}

bool game_is_diagadj(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->diagadj;
}

/********************* Jennifer *********************/
void game_undo(game g) { return; }

void game_redo(game g) { return; }

/********game********/

/********************* Jennifer *********************/

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  if (squares == NULL || nb_tents_row == NULL || nb_tents_col == NULL) {
    exit(EXIT_FAILURE);
  }
  game g = game_new_empty();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
    g->nb_tents_col[i] = nb_tents_col[i];
  }
  for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
    g->squares[j] = squares[j];
  }
  return g;
}

game game_new_empty(void) {
  game g = malloc(sizeof(game_s));
  if (g == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_row = malloc(sizeof(uint) * DEFAULT_SIZE);
  if (g->nb_tents_row == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_col = malloc(sizeof(uint) * DEFAULT_SIZE);
  if (g->nb_tents_col == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tents_row[i] = 0;
    g->nb_tents_col[i] = 0;
  }
  g->squares = malloc(sizeof(square) * DEFAULT_SIZE * DEFAULT_SIZE);
  if (g->squares == NULL) {
    fprintf(stderr, "not enough memory!\n");
    exit(EXIT_FAILURE);
  }
  for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
    g->squares[j] = EMPTY;
  }
  return g;
}

game game_copy(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  game g1 = game_new_empty();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g1->nb_tents_row[i] = game_get_expected_nb_tents_row(g, i);
    g1->nb_tents_col[i] = game_get_expected_nb_tents_col(g, i);
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      g1->squares[(i * DEFAULT_SIZE + j)] = game_get_square(g, i, j);
    }
  }
  return g1;
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    fprintf(stderr, "at least one of the parameters isn't valid!\n");
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g1->nb_rows; i++) {
    if (g1->nb_tents_row[i] != g2->nb_tents_row[i]) {
      return false;
    }
  }
  for (uint x = 0; x < g1->nb_cols; x++) {
    if (g1->nb_tents_col[x] != g2->nb_tents_col[x]) {
      return false;
    }
  }
  for (uint j = 0; j < g1->nb_rows * g1->nb_cols; j++) {
    if (g1->squares[j] != g2->squares[j]) {
      return false;
    }
  }
  return true;
}

void game_delete(game g) {
  if (g == NULL) {
    fprintf(stderr, "parameter not valid!\n");
    return;
  }
  free(g->squares);
  g->squares = NULL;
  free(g->nb_tents_col);
  g->nb_tents_col = NULL;
  free(g->nb_tents_row);
  g->nb_tents_row = NULL;
  free(g);
  g = NULL;
}

void game_set_square(game g, uint i, uint j, square s) {
  if (g == NULL || i >= DEFAULT_SIZE || j >= DEFAULT_SIZE || i < 0 || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->squares[i * DEFAULT_SIZE + j] = s;
}

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
  if (g == NULL || i >= DEFAULT_SIZE || i < 0) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k = 0; k < DEFAULT_SIZE; k++) {
    if (game_get_square(g, i, k) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= DEFAULT_SIZE || j < 0) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k = 0; k < DEFAULT_SIZE; k++) {
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

void game_play_move(game g, uint i, uint j, square s) {
  if (g == NULL || i >= DEFAULT_SIZE || j >= DEFAULT_SIZE || s == TREE ||
      game_get_square(g, i, j) == TREE) {
    exit(EXIT_FAILURE);
  }
  game_set_square(g, i, j, s);
}

/********************* Guillaume *********************/

int game_check_move(cgame g, uint i, uint j, square s) {
  if (g == NULL || i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }

  if ((game_get_square(g, i, j) == TREE) || (s == TREE)) {
    return ILLEGAL;
  }
  /* LOSING MOVE */

  /* Analyse si tente adjacente à une tente */
  if (i == 0 && j > 0 && j < DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i + 1, j + 1) == TENT ||
       game_get_square(g, i + 1, j - 1) == TENT)) {
    return LOSING;  // tent i==0 bordure j exclue
  }

  if (i > 0 && i < DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 &&
      s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i - 1, j - 1) == TENT ||
       game_get_square(g, i + 1, j + 1) == TENT ||
       game_get_square(g, i + 1, j - 1) == TENT ||
       game_get_square(g, i - 1, j + 1) ==
           TENT)) {  // tent toute bordure exclue
    return LOSING;
  }

  if (i == 0 && j == 0 && s == TENT &&
      (game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i + 1, j + 1) == TENT)) {  // tent en haut a gauche
    return LOSING;
  }

  if (i > 0 && j == 0 && i < DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i + 1, j + 1) == TENT ||
       game_get_square(g, i - 1, j + 1) ==
           TENT)) {  // tent j==0 toute bordure i exclue
    return LOSING;
  }

  if (i == DEFAULT_SIZE - 1 && j == 0 && s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i - 1, j + 1) == TENT)) {  // tent en bas a gauche
    return LOSING;
  }

  if (i == DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i, j + 1) == TENT ||
       game_get_square(g, i - 1, j - 1) == TENT ||
       game_get_square(g, i - 1, j + 1) ==
           TENT)) {  // tent i==DEFAULT_SIZE-1 bordure j exclue
    return LOSING;
  }

  if (i == DEFAULT_SIZE - 1 && j == DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i - 1, j - 1) == TENT)) {  // tent en bas a droite
    return LOSING;
  }

  if (i > 0 && j == DEFAULT_SIZE - 1 && i < DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i - 1, j) == TENT ||
       game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i + 1, j - 1) == TENT ||
       game_get_square(g, i - 1, j - 1) ==
           TENT)) {  // tent j==DEFAULT_SIZE-1 bordure i exclue
    return LOSING;
  }

  if (i == 0 && j == DEFAULT_SIZE - 1 && s == TENT &&
      (game_get_square(g, i + 1, j) == TENT ||
       game_get_square(g, i, j - 1) == TENT ||
       game_get_square(g, i + 1, j - 1) == TENT)) {  // tent en haut a droite
    return LOSING;
  }
  //---Analyse arbre présent au moins une fois autour d'une tente donnée---//

  if (i == 0 && s == TENT && j > 0 && j < DEFAULT_SIZE - 1 &&
      game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j - 1) != TREE &&
      game_get_square(g, i, j + 1) != TREE) {  // tent i==0 bordure j exclue
    return LOSING;
  }
  if (i > 0 && s == TENT && i < DEFAULT_SIZE - 1 && j > 0 &&
      j < DEFAULT_SIZE - 1 && game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j - 1) != TREE &&
      game_get_square(g, i, j + 1) != TREE) {  // tent toute bordure exclue
    return LOSING;
  }
  if (i == 0 && s == TENT && j == 0 && game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j + 1) != TREE) {  // tent en haut a gauche
    return LOSING;
  }
  if (i > 0 && s == TENT && j == 0 && i < DEFAULT_SIZE - 1 &&
      game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j + 1) !=
          TREE) {  // tent j==0 toute bordure i exclue
    return LOSING;
  }
  if (i == DEFAULT_SIZE - 1 && s == TENT && j == 0 &&
      game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i, j + 1) != TREE) {  // tent en bas a gauche
    return LOSING;
  }
  if (i == DEFAULT_SIZE - 1 && s == TENT && j > 0 && j < DEFAULT_SIZE - 1 &&
      game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i, j - 1) != TREE &&
      game_get_square(g, i, j + 1) !=
          TREE) {  // tent i==DEFAULT_SIZE-1 bordure j exclue
    return LOSING;
  }
  if (i == DEFAULT_SIZE - 1 && s == TENT && j == DEFAULT_SIZE - 1 &&
      game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i, j - 1) != TREE) {  // tent en bas a droite
    return LOSING;
  }
  if (i > 0 && j == DEFAULT_SIZE - 1 && s == TENT && i < DEFAULT_SIZE - 1 &&
      game_get_square(g, i - 1, j) != TREE &&
      game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j - 1) !=
          TREE) {  // tent j==DEFAULT_SIZE-1 bordure i exclue
    return LOSING;
  }
  if (i == 0 && j == DEFAULT_SIZE - 1 && s == TENT &&
      game_get_square(g, i + 1, j) != TREE &&
      game_get_square(g, i, j - 1) != TREE) {  // tent en haut a droite
    return LOSING;
  }

  /* Analyse si arbre adj grass puis arbre entouré de grass */

  if (i == 0 && j == 0 && s == GRASS) {  // grass placé coin gauche haut
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j + 1) == GRASS ||
         game_get_square(g, i + 1, j + 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      // analyse arbre i+1 de grass
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == GRASS ||
         game_get_square(g, i + 1, j + 1) == TREE) &&
        (game_get_square(g, i, j + 2) == GRASS ||
         game_get_square(g, i, j + 2) == TREE)) {
      // analyse arbre j+1 de grass
      return LOSING;
    }
  }

  if (i == 0 && j == 1 &&
      s == GRASS) {  // grass placé coin gauche haut +1 horizontal
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE) &&
        (game_get_square(g, i + 1, j + 1) == GRASS ||
         game_get_square(g, i + 1, j + 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 0 && j > 1 && j < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne du haut (i==0), "coin+" exclue
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 0 && j == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin droit haut -1 horizontal
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE) &&
        (game_get_square(g, i + 1, j + 1) == GRASS ||
         game_get_square(g, i + 1, j + 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 0 && j == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin droit haut
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      return LOSING;
    }
  }

  if (i == 1 && j == 0 &&
      s == GRASS) {  // grass placé coin gauche haut +1 verticale
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 1 && j == 1 && s == GRASS) {  // grass placé coin gauche haut +1h +1v
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 1 && j > 1 && j < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé 2eme ligne du haut (i==1), "coin+" exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 1 && j == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin droit haut -1h +1v
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (i == 1 && j == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin droit haut +1 verticale
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      return LOSING;
    }
  }

  if (j == 0 && i > 1 && i < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne gauche (j==0), "coin+" exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS)) {
      return LOSING;
    }
  }

  if (j == 1 && i > 1 && i < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé 2eme ligne gauche (j==1), "coin+" exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j > 1 && j < DEFAULT_SIZE - 2 && i > 1 && i < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé "centre jeu", "coin+" exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 2 && i > 1 && i < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne droite-1 (j==DEFAULT_SIZE-2), "coin+"
                     // exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 2, j) == TREE ||
         game_get_square(g, i + 2, j) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 1 && i > 1 && i < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne droite (j==DEFAULT_SIZE-1), "coin+"
                     // exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == GRASS ||
         game_get_square(g, i + 1, j - 1) == TREE) &&
        (game_get_square(g, i + 2, j) == GRASS ||
         game_get_square(g, i + 2, j) == TREE)) {
      return LOSING;
    }
  }

  if (j == 0 && i == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin gauche bas -1v
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == 1 && i == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin gauche bas -1v +1h
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (i == DEFAULT_SIZE - 2 && j > 1 && j < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne du bas-1 (i==DEFAULT_SIZE-2), "coin+"
                     // exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 2 && i == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin droit bas -1v -1h
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i + 1, j + 1) == TREE ||
         game_get_square(g, i + 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 1 && i == DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé coin droit bas -1v
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS) &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i + 1, j) == TREE &&
        (game_get_square(g, i + 1, j - 1) == TREE ||
         game_get_square(g, i + 1, j - 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == 0 && i == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin gauche bas
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS)) {
      return LOSING;
    }
  }

  if (j == 1 && i == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin gauche bas +1h
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS)) {
      return LOSING;
    }
  }

  if (i == DEFAULT_SIZE - 1 && j > 1 && j < DEFAULT_SIZE - 2 &&
      s == GRASS) {  // grass placé ligne du bas (i==DEFAULT_SIZE-1), "coin+"
                     // exclue
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS) &&
        (game_get_square(g, i, j + 2) == TREE ||
         game_get_square(g, i, j + 2) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 2 && i == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin droit bas -1h
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS) &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j + 1) == TREE &&
        (game_get_square(g, i - 1, j + 1) == TREE ||
         game_get_square(g, i - 1, j + 1) == GRASS)) {
      return LOSING;
    }
  }

  if (j == DEFAULT_SIZE - 1 && i == DEFAULT_SIZE - 1 &&
      s == GRASS) {  // grass placé coin droit bas
    if (game_get_square(g, i - 1, j) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i - 2, j) == TREE ||
         game_get_square(g, i - 2, j) == GRASS)) {
      return LOSING;
    }
    if (game_get_square(g, i, j - 1) == TREE &&
        (game_get_square(g, i - 1, j - 1) == TREE ||
         game_get_square(g, i - 1, j - 1) == GRASS) &&
        (game_get_square(g, i, j - 2) == TREE ||
         game_get_square(g, i, j - 2) == GRASS)) {
      return LOSING;
    }
  }

  /* Analyse si tente non attendue dans une row/col */
  if (s == TENT && (game_get_expected_nb_tents_col(g, j) == 0 ||
                    game_get_expected_nb_tents_row(g, i) == 0)) {
    return LOSING;
  }
  if (s == TENT && ((game_get_expected_nb_tents_row(g, i) <
                     game_get_current_nb_tents_row(g, i)) ||
                    (game_get_expected_nb_tents_col(g, j) <
                     game_get_current_nb_tents_col(g, j)))) {
    return LOSING;
  }

  /* Analyse si GRASS placé alors que le nombre de empty n'est pas suffisant
   * pour placer les tentes attendues */

  /*Compteur de EMPTY colonne*/
  uint c = 0;
  for (uint x = 0; x < DEFAULT_SIZE; x++) {
    if (game_get_square(g, x, j) == EMPTY) {
      c++;
    }
  }
  /*Compteur de EMPTY ligne*/
  uint d = 0;
  for (uint y = 0; y < DEFAULT_SIZE; y++) {
    if (game_get_square(g, i, y) == EMPTY) {
      d++;
    }
  }

  if (s == GRASS && ((d <= (game_get_expected_nb_tents_row(g, i) -
                            game_get_current_nb_tents_row(g, i))) ||
                     (c <= (game_get_expected_nb_tents_col(g, j) -
                            game_get_current_nb_tents_col(g, j))))) {
    return LOSING;
  }
  return REGULAR;
}

bool game_is_over(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  uint c = 0;
  //------------------------------------Analyse si tente présente
  // autour(+diagonale) d'une tente
  // donnée-----------------------------------------//
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (i == 0 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i + 1, j + 1) == TENT ||
           game_get_square(g, i + 1, j - 1) == TENT)) {
        return false;  // tent i==0 bordure j exclue
      }
      if (i > 0 && i < DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i - 1, j - 1) == TENT ||
           game_get_square(g, i + 1, j + 1) == TENT ||
           game_get_square(g, i + 1, j - 1) == TENT ||
           game_get_square(g, i - 1, j + 1) ==
               TENT)) {  // tent toute bordure exclue
        return false;
      }
      if (i == 0 && j == 0 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i + 1, j + 1) ==
               TENT)) {  // tent en haut a gauche
        return false;
      }
      if (i > 0 && j == 0 && i < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i + 1, j + 1) == TENT ||
           game_get_square(g, i - 1, j + 1) ==
               TENT)) {  // tent j==0 toute bordure i exclue
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j == 0 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i - 1, j + 1) == TENT)) {  // tent en bas a gauche
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           game_get_square(g, i - 1, j - 1) == TENT ||
           game_get_square(g, i - 1, j + 1) ==
               TENT)) {  // tent i==DEFAULT_SIZE-1 bordure j exclue
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j == DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i - 1, j - 1) == TENT)) {  // tent en bas a droite
        return false;
      }
      if (i > 0 && j == DEFAULT_SIZE - 1 && i < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i + 1, j - 1) == TENT ||
           game_get_square(g, i - 1, j - 1) ==
               TENT)) {  // tent j==DEFAULT_SIZE-1 bordure i exclue
        return false;
      }
      if (i == 0 && j == DEFAULT_SIZE - 1 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i + 1, j - 1) ==
               TENT)) {  // tent en haut a droite
        return false;
      }
      //------------------------------------Analyse arbre présent au moins une
      // fois autour d'une tente
      // donnée-----------------------------------------//

      if (i == 0 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE &&
          game_get_square(g, i, j + 1) != TREE) {  // tent i==0 bordure j exclue
        return false;
      }
      if (i > 0 && i < DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE &&
          game_get_square(g, i, j + 1) != TREE) {  // tent toute bordure exclue
        return false;
      }
      if (i == 0 && j == 0 && game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i, j + 1) != TREE) {  // tent en haut a gauche
        return false;
      }
      if (i > 0 && j == 0 && i < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j + 1) !=
              TREE) {  // tent j==0 toute bordure i exclue
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j == 0 &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i, j + 1) != TREE) {  // tent en bas a gauche
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j > 0 && j < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE &&
          game_get_square(g, i, j + 1) !=
              TREE) {  // tent i==DEFAULT_SIZE-1 bordure j exclue
        return false;
      }
      if (i == DEFAULT_SIZE - 1 && j == DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE) {  // tent en bas a droite
        return false;
      }
      if (i > 0 && j == DEFAULT_SIZE - 1 && i < DEFAULT_SIZE - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j - 1) !=
              TREE) {  // tent j==DEFAULT_SIZE-1 bordure i exclue
        return false;
      }
      if (i == 0 && j == DEFAULT_SIZE - 1 &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i, j - 1) != TREE) {  // tent en haut a droite
        return false;
      }
      if (game_get_current_nb_tents_row(g, i) !=
          game_get_expected_nb_tents_row(g, i)) {
        return false;
      }
      if (game_get_current_nb_tents_col(g, j) !=
          game_get_expected_nb_tents_col(g, j)) {
        return false;
      }
      if (game_get_square(g, i, j) == TREE) {
        cpt = cpt + 1;
      }
      if (game_get_square(g, i, j) == TENT) {
        c = c + 1;
      }
    }
  }
  if (cpt != c) {
    return false;
  }
  if (game_get_expected_nb_tents_all(g) < game_get_current_nb_tents_all(g)) {
    return false;
  }
  return true;
}
void game_fill_grass_row(game g, uint i) {
  if (g == NULL || i >= DEFAULT_SIZE) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_fill_grass_col(game g, uint j) {
  if (g == NULL || j >= DEFAULT_SIZE) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_restart(game g) {
  if (g == NULL) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != TREE) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
}