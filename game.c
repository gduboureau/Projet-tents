#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_ext.h"
#include "queue.c"

typedef struct game_s {
  square *squares;
  uint *nb_tents_row;
  uint *nb_tents_col;
  uint nb_rows;
  uint nb_cols;
  bool wrapping;
  bool diagadj;
  queue *pile1;
  queue *pile2;
} game_s;

typedef struct coup {
  square s;
  uint i;
  uint j;
} coup;

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
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;
  for (uint i = 0; i < g->nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint i = 0; i < g->nb_cols; i++) {
    g->nb_tents_col[i] = nb_tents_col[i];
  }
  for (uint j = 0; j < g->nb_rows * g->nb_cols; j++) {
    g->squares[j] = squares[j];
  }
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
  g->pile1 = queue_new();
  g->pile2 = queue_new();
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
  for (uint i = 0; i < g->nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint i = 0; i < g->nb_cols; i++) {
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
void game_undo(game g) {
  if (!queue_is_empty(g->pile1)) {
    coup *data = (coup *)queue_pop_head(g->pile1);
    queue_push_head(g->pile2, data);
    coup *data0 = (coup *)queue_pop_head(g->pile1);
    game_set_square(g, data0->i, data0->j, data0->s);
    free(data0);
  }
}

void game_redo(game g) {
  if (!queue_is_empty(g->pile2)) {
    coup *data = (coup *)queue_pop_head(g->pile2);
    game_set_square(g, data->i, data->j, data->s);
    free(data);
  }
}

/********game********/

/********************* Jennifer *********************/

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  if (squares == NULL || nb_tents_row == NULL || nb_tents_col == NULL) {
    exit(EXIT_FAILURE);
  }
  game g = game_new_empty();
  for (uint i = 0; i < g->nb_rows; i++) {
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint x = 0; x < g->nb_cols; x++) {
    g->nb_tents_col[x] = nb_tents_col[x];
  }
  for (uint j = 0; j < g->nb_rows * g->nb_cols; j++) {
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
  g->nb_cols = DEFAULT_SIZE;
  g->nb_rows = DEFAULT_SIZE;
  g->wrapping = false;
  g->diagadj = false;
  g->pile1 = queue_new();
  g->pile2 = queue_new();
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
  game g1 = game_new_empty_ext(g->nb_rows, g->nb_cols, g->wrapping, g->diagadj);
  for (uint i = 0; i < g->nb_rows; i++) {
    g1->nb_tents_row[i] = game_get_expected_nb_tents_row(g, i);
    for (uint j = 0; j < g->nb_cols; j++) {
      g1->nb_tents_col[i] = game_get_expected_nb_tents_col(g, i);
      g1->squares[(i * g->nb_cols + j)] = game_get_square(g, i, j);
    }
  }
  return g1;
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    fprintf(stderr, "at least one of the parameters isn't valid!\n");
    exit(EXIT_FAILURE);
  }
  if (g1->wrapping != g2->wrapping || g1->diagadj != g2->diagadj) {
    return false;
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
  while (!queue_is_empty(g->pile1)) {
    coup *data = (coup *)queue_pop_head(g->pile1);
    free(data);
  }
  queue_clear_full(g->pile1, &free);
  queue_free(g->pile1);
  g->pile1 = NULL;
  while (!queue_is_empty(g->pile2)) {
    coup *data1 = (coup *)queue_pop_head(g->pile2);
    free(data1);
  }
  queue_clear_full(g->pile2, &free);
  queue_free(g->pile2);
  g->pile2 = NULL;
  free(g);
  g = NULL;
}

void game_set_square(game g, uint i, uint j, square s) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols || i < 0 || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->squares[(i * g->nb_cols) + j] = s;
}

/********************* Hugo *********************/

square game_get_square(cgame g, uint i, uint j) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols || i < 0 || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->squares[(i * g->nb_cols) + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  if (g == NULL || i >= g->nb_rows || i < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_row[i] = nb_tents;
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  if (g == NULL || j >= g->nb_cols || j < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  g->nb_tents_col[j] = nb_tents;
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  if (g == NULL || i >= g->nb_rows || i < 0) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_tents_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= g->nb_cols || j < 0) {
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
  for (uint j = 0; j < g->nb_rows; j++) {
    cpt += game_get_expected_nb_tents_row(g, j);
  }
  return cpt;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  if (g == NULL || i >= g->nb_rows || i < 0) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k = 0; k < g->nb_cols; k++) {
    if (game_get_square(g, i, k) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= g->nb_cols || j < 0) {
    exit(EXIT_FAILURE);
  }
  uint cpt = 0;
  for (uint k = 0; k < g->nb_rows; k++) {
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
  for (uint j = 0; j < g->nb_rows; j++) {
    cpt += game_get_current_nb_tents_row(g, j);
  }
  return cpt;
}

void game_play_move(game g, uint i, uint j, square s) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols || s == TREE ||
      game_get_square(g, i, j) == TREE) {
    exit(EXIT_FAILURE);
  }
  if (!queue_is_empty(g->pile2)) {
    queue_clear_full(g->pile2, &free);
  }
  coup p0;
  p0.s = game_get_square(g, i, j);
  p0.i = i;
  p0.j = j;
  coup *data0 = (coup *)malloc(sizeof(coup));
  assert(data0);
  *data0 = p0;
  queue_push_head(g->pile1, data0);
  coup p1;
  p1.s = s;
  p1.i = i;
  p1.j = j;
  coup *data = (coup *)malloc(sizeof(coup));
  assert(data);
  *data = p1;
  queue_push_head(g->pile1, data);
  game_set_square(g, i, j, s);
}

/********************* Guillaume *********************/

typedef struct coor {
  uint ligne;
  uint colonne;
} coor;

static coor make_coor(uint l, uint c) {
  coor coor;
  coor.ligne = l;
  coor.colonne = c;
  return coor;
}
typedef enum { NONE, NORTH, SOUTH, WEST, EAST, NW, NE, SW, SE } dir;

static coor dir_to_coor(dir d) {
  switch (d) {
    case NONE:
      return make_coor(0, 0);
    case NORTH:
      return make_coor(0, -1);
    case SOUTH:
      return make_coor(0, 1);
    case WEST:
      return make_coor(-1, 0);
    case EAST:
      return make_coor(1, 0);
    case NW:
      return make_coor(-1, -1);
    case NE:
      return make_coor(1, -1);
    case SW:
      return make_coor(-1, 1);
    case SE:
      return make_coor(1, 1);
  }
  exit(EXIT_FAILURE);
}

static dir coor_to_dir(coor coor) {
  uint l = coor.ligne;
  uint c = coor.colonne;
  switch (l) {
    case 0:
      switch (c) {
        case 0:
          return NONE;
        case -1:
          return NORTH;
        case 1:
          return SOUTH;
      }
    case -1:
      switch (c) {
        case 0:
          return WEST;
        case -1:
          return NW;
        case 1:
          return SW;
      }
    case 1:
      switch (c) {
        case 0:
          return EAST;
        case -1:
          return NE;
        case 1:
          return SE;
      }
  }
  exit(EXIT_FAILURE);
}

static bool ligne_correcte(cgame g, uint l) {
  return l < game_nb_rows(g) && l >= 0;
}
static bool colonne_correcte(cgame g, uint c) {
  return c < game_nb_cols(g) && c >= 0;
}

static bool coordonnee_ok(cgame g, coor coor) {
  return ligne_correcte(g, coor.ligne) && colonne_correcte(g, coor.colonne);
}

static coor next_coor(cgame g, coor c, coor c2) {
  int li = c.ligne + c2.ligne;
  int co = c.colonne + c2.colonne;
  if (g->wrapping) {
    li = li % g->nb_rows;
    co = co % g->nb_cols;
  }
  return make_coor(li, co);
}

static coor dir_next_coor(cgame g, coor coor, dir dir) {
  return next_coor(g, coor, dir_to_coor(dir));
}

static bool correct_next_coor(cgame g, coor c, dir d) {
  return coordonnee_ok(g, dir_next_coor(g, c, d));
}

static bool r1_tent_adj_tent(cgame g, uint x, uint y, square s) {
  if (s == TENT) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (correct_next_coor(g, make_coor(x, y),
                              coor_to_dir(make_coor(i, j)))) {
          printf("mots \n");
          if ((x + i + y + j != y + x) &&
              game_get_square(g, x + i, y + j) == TENT) {
            printf("pas mots \n");
            return false;
          }
        }
      }
    }
  }
  return true;
}

static bool r2_nb_tent_respecte(cgame g, square s) {
  for (uint i = 0; i < game_nb_rows(g); i++) {
    if (s == TENT && game_get_current_nb_tents_row(g, i) >
                         game_get_expected_nb_tents_row(g, i)) {
      return false;
    }
  }
  for (uint j = 0; j < game_nb_cols(g); j++) {
    if (s == TENT && game_get_current_nb_tents_col(g, j) >
                         game_get_expected_nb_tents_col(g, j)) {
      return false;
    }
  }
  return true;
}

static bool r3_tent_tree(cgame g, uint x, uint y, square s) {
  if (s == TENT) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (((i != -1 && j != -1) || (i != 1 && j != -1) || (i != -1 && j != 1) || (i != 1 && j != 1)) && (correct_next_coor(g, make_coor(x, y),
                              coor_to_dir(make_coor(i, j))))) {
          printf("mots \n");
          if ((x + i + y + j != y + x) &&
              game_get_square(g, x + i, y + j) == TREE) {
            printf("pas mots \n");
            return true;
          }
        }
      }
    }
  }
  return false;
}

static bool game_correct(cgame g, uint x, uint y, square s) {
  return r2_nb_tent_respecte(g, s) && r1_tent_adj_tent(g, x, y, s) && r3_tent_tree(g,x,y,s);
}

int game_check_move(cgame g, uint i, uint j, square s) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  if (!game_correct(g, i, j, s)) {
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
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (i == 0 && j > 0 && j < g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i + 1, j + 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i + 1, j - 1) == TENT &&
            game_is_diagadj(g) == false))) {
        return false;  // tent i==0 bordure j exclue
      }
      if (i > 0 && i < g->nb_rows - 1 && j > 0 && j < g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i - 1, j - 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i + 1, j + 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i + 1, j - 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i - 1, j + 1) == TENT &&
            game_is_diagadj(g) == false))) {  // tent toute bordure exclue
        return false;
      }
      if (i == 0 && j == 0 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i + 1, j + 1) == TENT &&
            game_is_diagadj(g) == false))) {  // tent en haut a gauche
        return false;
      }
      if (i > 0 && j == 0 && i < g->nb_rows - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i + 1, j + 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i - 1, j + 1) == TENT &&
            game_is_diagadj(g) ==
                false))) {  // tent j==0 toute bordure i exclue
        return false;
      }
      if (i == g->nb_rows - 1 && j == 0 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i - 1, j + 1) == TENT &&
            game_is_diagadj(g) == false))) {  // tent en bas a gauche
        return false;
      }
      if (i == g->nb_rows - 1 && j > 0 && j < g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           game_get_square(g, i, j + 1) == TENT ||
           (game_get_square(g, i - 1, j - 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i - 1, j + 1) == TENT &&
            game_is_diagadj(g) ==
                false))) {  // tent i==g->nb_rows-1 bordure j exclue
        return false;
      }
      if (i == g->nb_rows - 1 && j == g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           (game_get_square(g, i - 1, j - 1) == TENT &&
            game_is_diagadj(g) == false))) {  // tent en bas a droite
        return false;
      }
      if (i > 0 && j == g->nb_cols - 1 && i < g->nb_rows - 1 &&
          game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i - 1, j) == TENT ||
           game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           (game_get_square(g, i + 1, j - 1) == TENT &&
            game_is_diagadj(g) == false) ||
           (game_get_square(g, i - 1, j - 1) == TENT &&
            game_is_diagadj(g) ==
                false))) {  // tent j==g->nb_cols-1 bordure i exclue
        return false;
      }
      if (i == 0 && j == g->nb_cols - 1 && game_get_square(g, i, j) == TENT &&
          (game_get_square(g, i + 1, j) == TENT ||
           game_get_square(g, i, j - 1) == TENT ||
           (game_get_square(g, i + 1, j - 1) == TENT &&
            game_is_diagadj(g) == false))) {  // tent en haut a droite
        return false;
      }
      //------------------------------------Analyse arbre présent au moins une
      // fois autour d'une tente
      // donnée-----------------------------------------//

      if (i == 0 && j > 0 && j < g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE &&
          game_get_square(g, i, j + 1) != TREE) {  // tent i==0 bordure j exclue
        return false;
      }
      if (i > 0 && i < g->nb_rows - 1 && j > 0 && j < g->nb_cols - 1 &&
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
      if (i > 0 && j == 0 && i < g->nb_rows - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j + 1) !=
              TREE) {  // tent j==0 toute bordure i exclue
        return false;
      }
      if (i == g->nb_rows - 1 && j == 0 &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i, j + 1) != TREE) {  // tent en bas a gauche
        return false;
      }
      if (i == g->nb_rows - 1 && j > 0 && j < g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE &&
          game_get_square(g, i, j + 1) !=
              TREE) {  // tent i==g->nb_rows-1 bordure j exclue
        return false;
      }
      if (i == g->nb_rows - 1 && j == g->nb_cols - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i, j - 1) != TREE) {  // tent en bas a droite
        return false;
      }
      if (i > 0 && j == g->nb_cols - 1 && i < g->nb_rows - 1 &&
          game_get_square(g, i, j) == TENT &&
          game_get_square(g, i - 1, j) != TREE &&
          game_get_square(g, i + 1, j) != TREE &&
          game_get_square(g, i, j - 1) !=
              TREE) {  // tent j==g->nb_cols-1 bordure i exclue
        return false;
      }
      if (i == 0 && j == g->nb_cols - 1 &&
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
  if (g == NULL || i >= g->nb_cols) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (uint j = 0; j < g->nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      coup p0;
      p0.s = game_get_square(g, i, j);
      p0.i = i;
      p0.j = j;
      coup *data0 = (coup *)malloc(sizeof(coup));
      *data0 = p0;
      queue_push_head(g->pile1, data0);
      coup p1;
      p1.s = GRASS;
      p1.i = i;
      p1.j = j;
      coup *data = (coup *)malloc(sizeof(coup));
      *data = p1;
      queue_push_head(g->pile1, data);
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_fill_grass_col(game g, uint j) {
  if (g == NULL || j >= g->nb_cols) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (unsigned int i = 0; i < g->nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      coup p0;
      p0.s = game_get_square(g, i, j);
      p0.i = i;
      p0.j = j;
      coup *data0 = (coup *)malloc(sizeof(coup));
      *data0 = p0;
      queue_push_head(g->pile1, data0);
      coup p1;
      p1.s = GRASS;
      p1.i = i;
      p1.j = j;
      coup *data = (coup *)malloc(sizeof(coup));
      *data = p1;
      queue_push_head(g->pile1, data);
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_restart(game g) {
  if (g == NULL) {
    fprintf(stderr, "parameter not valid!\n");
    exit(EXIT_FAILURE);
  }
  for (unsigned int i = 0; i < g->nb_rows; i++) {
    for (unsigned int j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) != TREE) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
  if (!queue_is_empty(g->pile1)) {
    queue_clear_full(g->pile1, &free);
  }

  if (!queue_is_empty(g->pile2)) {
    queue_clear_full(g->pile2, &free);
  }
}