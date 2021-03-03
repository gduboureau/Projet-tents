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

// game_ext

game game_new_ext(uint nb_rows, uint nb_cols, square *squares,
                  uint *nb_tents_row, uint *nb_tents_col, bool wrapping,
                  bool diagadj) {
  assert(squares);
  assert(nb_tents_row);
  assert(nb_tents_col);
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
  assert(g);
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;
  g->pile1 = queue_new();
  g->pile2 = queue_new();
  g->nb_tents_row = malloc(sizeof(uint) * nb_rows);
  assert(g->nb_tents_row);
  g->nb_tents_col = malloc(sizeof(uint) * nb_cols);
  assert(g->nb_tents_col);
  for (uint i = 0; i < g->nb_rows; i++) {
    g->nb_tents_row[i] = 0;
  }
  for (uint i = 0; i < g->nb_cols; i++) {
    g->nb_tents_col[i] = 0;
  }

  g->squares = malloc(sizeof(square) * nb_rows * nb_cols);
  assert(g->squares);
  for (uint j = 0; j < nb_rows * nb_cols; j++) {
    g->squares[j] = EMPTY;
  }

  return g;
}

uint game_nb_rows(cgame g) {
  assert(g);
  return g->nb_rows;
}

uint game_nb_cols(cgame g) {
  assert(g);
  return g->nb_cols;
}

bool game_is_wrapping(cgame g) {
  assert(g);
  return g->wrapping;
}

bool game_is_diagadj(cgame g) {
  assert(g);
  return g->diagadj;
}

void game_undo(game g) {
  if (!queue_is_empty(g->pile1)) {
    coup *data =
        (coup *)queue_pop_head(g->pile1);  // on récupère le dernier coup joué
    queue_push_head(g->pile2, data);  // On sauvegarde ce coup dans la pile2
    coup *data0 =
        (coup *)queue_pop_head(g->pile1);  // On récupère les données de la case
                                           // avant de jouer le coup data
    game_set_square(g, data0->i, data0->j, data0->s);  // On annule le coup data
    free(data0);
  }
}

void game_redo(game g) {
  if (!queue_is_empty(g->pile2)) {
    coup *data =
        (coup *)queue_pop_head(g->pile2);  // on récupère le dernier coup annulé
    game_set_square(g, data->i, data->j, data->s);  // on rejoue le coup annulé
    free(data);
  }
}

// V1

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col) {
  assert(squares);
  assert(nb_tents_row);
  assert(nb_tents_col);
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
  assert(g);
  g->nb_cols = DEFAULT_SIZE;
  g->nb_rows = DEFAULT_SIZE;
  g->wrapping = false;
  g->diagadj = false;
  g->pile1 = queue_new();
  g->pile2 = queue_new();
  g->nb_tents_row = malloc(sizeof(uint) * DEFAULT_SIZE);
  assert(g->nb_tents_row);
  g->nb_tents_col = malloc(sizeof(uint) * DEFAULT_SIZE);
  assert(g->nb_tents_col);
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tents_row[i] = 0;
    g->nb_tents_col[i] = 0;
  }
  g->squares = malloc(sizeof(square) * DEFAULT_SIZE * DEFAULT_SIZE);
  assert(g->squares);
  for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
    g->squares[j] = EMPTY;
  }
  return g;
}

game game_copy(cgame g) {
  assert(g);
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
  assert(g1);
  assert(g2);
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
  assert(g);
  free(g->squares);
  g->squares = NULL;
  free(g->nb_tents_col);
  g->nb_tents_col = NULL;
  free(g->nb_tents_row);
  g->nb_tents_row = NULL;
  while (!queue_is_empty(g->pile1)) {
    coup *data = (coup *)queue_pop_head(
        g->pile1);  // on désempile chaque élément de la pile1
    free(data);
  }
  queue_clear_full(g->pile1, &free);  // on vide la pile1
  queue_free(g->pile1);
  g->pile1 = NULL;
  while (!queue_is_empty(g->pile2)) {
    coup *data1 = (coup *)queue_pop_head(
        g->pile2);  // on désempile chaque élément de la pile2
    free(data1);
  }
  queue_clear_full(g->pile2, &free);  // on vide la pile2
  queue_free(g->pile2);
  g->pile2 = NULL;
  free(g);
  g = NULL;
}

void game_set_square(game g, uint i, uint j, square s) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  assert(j < g->nb_cols);
  assert(j >= 0);
  g->squares[(i * g->nb_cols) + j] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  assert(j < g->nb_cols);
  assert(j >= 0);
  return g->squares[(i * g->nb_cols) + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  g->nb_tents_row[i] = nb_tents;
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  assert(g);
  assert(j < g->nb_cols);
  assert(j >= 0);
  g->nb_tents_col[j] = nb_tents;
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  return g->nb_tents_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  assert(g);
  assert(j < g->nb_cols);
  assert(j >= 0);
  return g->nb_tents_col[j];
}

uint game_get_expected_nb_tents_all(cgame g) {
  assert(g);
  uint cpt = 0;
  for (uint j = 0; j < g->nb_rows; j++) {
    cpt += game_get_expected_nb_tents_row(g, j);
  }
  return cpt;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  uint cpt = 0;
  for (uint k = 0; k < g->nb_cols; k++) {
    if (game_get_square(g, i, k) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  assert(g);
  assert(j < g->nb_cols);
  assert(j >= 0);
  uint cpt = 0;
  for (uint k = 0; k < g->nb_rows; k++) {
    if (game_get_square(g, k, j) == TENT) {
      cpt += 1;
    }
  }
  return cpt;
}

uint game_get_current_nb_tents_all(cgame g) {
  assert(g);
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
    queue_clear_full(g->pile2, &free);  // on vide la pile 2
  }
  coup p0;
  p0.s = game_get_square(g, i, j);
  p0.i = i;
  p0.j = j;
  coup *data0 = (coup *)malloc(sizeof(coup));
  assert(data0);
  *data0 = p0;
  queue_push_head(g->pile1,
                  data0);  // on sauvegarde les données de la case (i,j)
  coup p1;
  p1.s = s;
  p1.i = i;
  p1.j = j;
  coup *data = (coup *)malloc(sizeof(coup));
  assert(data);
  *data = p1;
  queue_push_head(g->pile1,
                  data);  // on sauvegarde les données du coup joué en (i,j)
  game_set_square(g, i, j, s);
}

typedef struct coor {  // coor = coordonnée
  uint ligne;
  uint colonne;
} coor;

static coor make_coor(uint l, uint c) {
  coor coor;
  coor.ligne = l;
  coor.colonne = c;
  return coor;
}
typedef enum {
  NONE,
  NORTH,
  SOUTH,
  WEST,
  EAST,
  NW,
  NE,
  SW,
  SE
} dir;  // dir = direction

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
    if (li == -1 && co == -1) {  // cas ou l'on cherche à regarder en haut à
                                 // gauche de la case (0,0)
      li = g->nb_rows - 1;
      co = g->nb_cols - 1;
      return make_coor(li, co);
    }
    if (li == -1 && co != -1) {  // cas ou l'on cherche à regarder en haut d'une
                                 // case situé sur la première ligne
      li = g->nb_rows - 1;
      co = co % g->nb_cols;
      return make_coor(li, co);
    }
    if (li != -1 && co == -1) {  // cas ou l'on cherche à regarder à gauche
                                 // d'une case situé sur la première colonne
      li = li % g->nb_rows;
      co = g->nb_cols - 1;
      return make_coor(li, co);
    } else {
      li = li % g->nb_rows;
      co = co % g->nb_cols;
      return make_coor(li, co);
    }
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
          coor coo_next = next_coor(g, make_coor(x, y), make_coor(i, j));
          if (g->diagadj == false) {
            if ((i != 0 ||
                 j != 0) &&  // test pour ne pas parcourir la case (x,y)
                game_get_square(g, coo_next.ligne, coo_next.colonne) == TENT) {
              return false;
            }
          } else {
            if ((i == 0 || j == 0) &&
                (i + j != 0) &&  // test pour ne pas parcourir la case (x,y) et
                                 // les diagonales
                game_get_square(g, coo_next.ligne, coo_next.colonne) == TENT) {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}

static bool r3_tent_next_to_tree(cgame g, uint x, uint y, square s) {
  int cpt = 0;  // compteur de case != arbre
  int cpt_case_correcte = 0;
  if (s == TENT) {
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if ((i == 0 || j == 0) &&
            (i + j != 0) &&  // test pour ne pas parcourir la case (x,y) et les
                             // diagonales
            (correct_next_coor(g, make_coor(x, y),
                               coor_to_dir(make_coor(i, j))))) {
          coor coo_next = next_coor(g, make_coor(x, y), make_coor(i, j));
          cpt_case_correcte++;
          if (game_get_square(g, coo_next.ligne, coo_next.colonne) != TREE) {
            cpt++;
          }
        }
      }
    }
  }
  if (cpt_case_correcte == 2) {
    return cpt != 2;
  }
  if (cpt_case_correcte == 3) {
    return cpt != 3;
  }
  return cpt !=
         4;  // return true si il y a au moins un arbre autour de la case (x,y)
}

#define INDEX(g, i, j) ((i) * (g->nb_cols) + (j))
#define SQUARE(g, i, j) ((g)->squares[(INDEX(g, i, j))])

typedef enum { HERE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT } direction;

char _square2str(square s) {
  if (s == EMPTY)
    return ' ';
  else if (s == TREE)
    return 'x';
  else if (s == TENT)
    return '*';
  else if (s == GRASS)
    return '-';
  else
    return '?';
}

/* ************************************************************************** */

int _str2square(char c) {
  if (c == ' ')
    return EMPTY;
  else if (c == 'x')
    return TREE;
  else if (c == '*')
    return TENT;
  else if (c == '-')
    return GRASS;
  else
    return -1;  // invalid
}

/* ************************************************************************** */

bool _inside(cgame g, int i, int j) {
  assert(g);
  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  if (i < 0 || j < 0 || i >= (int)g->nb_rows || j >= (int)g->nb_cols) return false;
  return true;
}

/* ************************************************************************** */

bool _inside_neigh(cgame g, int i, int j, direction dir) {
  if (dir == HERE) return _inside(g, i, j);
  if (dir == UP) return _inside(g, i - 1, j);
  if (dir == DOWN) return _inside(g, i + 1, j);
  if (dir == LEFT) return _inside(g, i, j - 1);
  if (dir == RIGHT) return _inside(g, i, j + 1);
  if (dir == UP_LEFT) return _inside(g, i - 1, j - 1);
  if (dir == UP_RIGHT) return _inside(g, i - 1, j + 1);
  if (dir == DOWN_LEFT) return _inside(g, i + 1, j - 1);
  if (dir == DOWN_RIGHT) return _inside(g, i + 1, j + 1);
  return false;
}

/* ************************************************************************** */

bool _test(cgame g, int i, int j, square s) {
  assert(g);
  assert(s == EMPTY || s == TREE || s == TENT || s == GRASS);
  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  if (!_inside(g, i, j)) return false;
  return (SQUARE(g, i, j) == s);
}

/* ************************************************************************** */

bool _test_neigh(cgame g, int i, int j, square s, direction dir) {
  if (dir == HERE) return _test(g, i, j, s);
  if (dir == UP) return _test(g, i - 1, j, s);
  if (dir == DOWN) return _test(g, i + 1, j, s);
  if (dir == LEFT) return _test(g, i, j - 1, s);
  if (dir == RIGHT) return _test(g, i, j + 1, s);
  if (dir == UP_LEFT) return _test(g, i - 1, j - 1, s);
  if (dir == UP_RIGHT) return _test(g, i - 1, j + 1, s);
  if (dir == DOWN_LEFT) return _test(g, i + 1, j - 1, s);
  if (dir == DOWN_RIGHT) return _test(g, i + 1, j + 1, s);
  return false;
}

/* ************************************************************************** */

bool _neigh(cgame g, uint i, uint j, square s, bool diag) {
  assert(g);
  // assert(i < g->nb_rows && j < g->nb_cols);
  assert(s == EMPTY || s == TREE || s == TENT || s == GRASS);

  // orthogonally
  if (_test_neigh(g, i, j, s, UP)) return true;
  if (_test_neigh(g, i, j, s, DOWN)) return true;
  if (_test_neigh(g, i, j, s, LEFT)) return true;
  if (_test_neigh(g, i, j, s, RIGHT)) return true;

  // diagonally
  if (diag) {
    if (_test_neigh(g, i, j, s, UP_LEFT)) return true;
    if (_test_neigh(g, i, j, s, UP_RIGHT)) return true;
    if (_test_neigh(g, i, j, s, DOWN_LEFT)) return true;
    if (_test_neigh(g, i, j, s, DOWN_RIGHT)) return true;
  }

  return false;
}

/* ************************************************************************** */

uint _neigh_size(cgame g, uint i, uint j, bool diag) {
  assert(g);
  // assert(i < g->nb_rows && j < g->nb_cols);

  uint size = 0;

  // orthogonally
  if (_inside_neigh(g, i, j, UP)) size++;
  if (_inside_neigh(g, i, j, DOWN)) size++;
  if (_inside_neigh(g, i, j, LEFT)) size++;
  if (_inside_neigh(g, i, j, RIGHT)) size++;

  // diagonally
  if (diag) {
    if (_inside_neigh(g, i, j, UP_LEFT)) size++;
    if (_inside_neigh(g, i, j, UP_RIGHT)) size++;
    if (_inside_neigh(g, i, j, DOWN_LEFT)) size++;
    if (_inside_neigh(g, i, j, DOWN_RIGHT)) size++;
  }

  return size;
}

/* ************************************************************************** */

uint _neigh_count(cgame g, uint i, uint j, square s, bool diag) {
  assert(g);
  // assert(i < g->nb_rows && j < g->nb_cols);

  uint count = 0;

  // orthogonally
  if (_test_neigh(g, i, j, s, UP)) count++;
  if (_test_neigh(g, i, j, s, DOWN)) count++;
  if (_test_neigh(g, i, j, s, LEFT)) count++;
  if (_test_neigh(g, i, j, s, RIGHT)) count++;

  // diagonally
  if (diag) {
    if (_test_neigh(g, i, j, s, UP_LEFT)) count++;
    if (_test_neigh(g, i, j, s, UP_RIGHT)) count++;
    if (_test_neigh(g, i, j, s, DOWN_LEFT)) count++;
    if (_test_neigh(g, i, j, s, DOWN_RIGHT)) count++;
  }

  return count;
}

/* ************************************************************************** */

uint _nb_squares_row(cgame g, uint i, square s) {
  assert(g);
  assert(i < g->nb_rows);
  uint nb_squares = 0;
  for (uint j = 0; j < g->nb_cols; j++)
    if (SQUARE(g, i, j) == s) nb_squares++;
  return nb_squares;
}

/* ************************************************************************** */

uint _nb_squares_col(cgame g, uint j, square s) {
  assert(g);
  assert(j < g->nb_cols);
  uint nb_squares = 0;
  for (uint i = 0; i < g->nb_rows; i++)
    if (SQUARE(g, i, j) == s) nb_squares++;
  return nb_squares;
}

/* ************************************************************************** */

uint _nb_squares(cgame g, square s) {
  assert(g);
  assert(s == EMPTY || s == TREE || s == TENT || s == GRASS);
  uint nb_squares = 0;
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++)
      if (SQUARE(g, i, j) == s) nb_squares++;
  return nb_squares;
}


int game_check_move(cgame g, uint i, uint j, square s) {
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  assert(s == EMPTY || s == TREE || s == TENT || s == GRASS);
  square cs = SQUARE(g, i, j);  // current square at direction (i,j)

  // === check rule 0 === //

  if (s == TREE) return ILLEGAL;
  if (cs == TREE) return ILLEGAL;

  // No need to check rules, if I play an EMPTY move!
  if (s == EMPTY) return REGULAR;

  // Nota Bene: starting from this line, s is either TENT or GRASS.

  // update nb empty & tent squares, after playig move s
  int delta_empty_squares = ((s == EMPTY) ? 1 : 0) - ((cs == EMPTY) ? 1 : 0);
  int nb_empty_squares_row = _nb_squares_row(g, i, EMPTY) + delta_empty_squares;
  int nb_empty_squares_col = _nb_squares_col(g, j, EMPTY) + delta_empty_squares;
  int nb_empty_squares_all = _nb_squares(g, EMPTY) + delta_empty_squares;
  int delta_tent_squares = ((s == TENT) ? 1 : 0) - ((cs == TENT) ? 1 : 0);
  int nb_tent_squares_row = _nb_squares_row(g, i, TENT) + delta_tent_squares;
  int nb_tent_squares_col = _nb_squares_col(g, j, TENT) + delta_tent_squares;
  int nb_tent_squares_all = _nb_squares(g, TENT) + delta_tent_squares;
  int delta_grass_squares = ((s == GRASS) ? 1 : 0) - ((cs == GRASS) ? 1 : 0);
  // int nb_grass_squares_row = _nb_squares_row(g, i, GRASS) + delta_grass_squares;
  // int nb_grass_squares_col = _nb_squares_col(g, j, GRASS) + delta_grass_squares;
  // int nb_grass_squares_all = _nb_squares(g, GRASS) + delta_grass_squares;

  // === check rule 1 === //

  // no two tents are adjacent, even diagonally
  if ((s == TENT) && _neigh(g, i, j, TENT, !g->diagadj)) return LOSING;

  // === check rule 2 === //

  // 2.1) too much tents in a row or col
  // just check this rule, if I play TENT
  if (s == TENT) {
    if (nb_tent_squares_row > g->nb_tents_row[i]) return LOSING;
    if (nb_tent_squares_col > g->nb_tents_col[j]) return LOSING;
  }

  // 2.2) not enough empty squares in a row or col
  // just check this rule, if I play GRASS
  if (s == GRASS) {
    if ((nb_empty_squares_row + nb_tent_squares_row) < g->nb_tents_row[i]) return LOSING;
    if ((nb_empty_squares_col + nb_tent_squares_col) < g->nb_tents_col[j]) return LOSING;
  }

  // === check rule 3 === //

  // 3.1) too much tents
  // just check this rule, if I play TENT
  if (s == TENT) {
    if (nb_tent_squares_all > _nb_squares(g, TREE)) return LOSING;
  }

  // 3.2) not enough empty squares to place all tents
  // just check this rule, if I play GRASS
  if (s == GRASS) {
    if ((nb_empty_squares_all + nb_tent_squares_all) < _nb_squares(g, TREE)) return LOSING;
  }

  // === check rule 4 === //

  // 4.1) each tent must be orthogonally adjacent to at least one tree.
  if ((s == TENT) && !_neigh(g, i, j, TREE, false)) return LOSING;

  // 4.2) each tree must be orthogonally adjacent to at least one tent.

  // So, if I play grass, it should not prevent an adjacent tree to have at
  // least a tent in its neighborhood...
  if (s == GRASS) {
    if (_test_neigh(g, i, j, TREE, UP) &&
        ((_neigh_count(g, i - 1, j, GRASS, false) + delta_grass_squares) == _neigh_size(g, i - 1, j, false)))
      return LOSING;
    if (_test_neigh(g, i, j, TREE, DOWN) &&
        ((_neigh_count(g, i + 1, j, GRASS, false) + delta_grass_squares) == _neigh_size(g, i + 1, j, false)))
      return LOSING;
    if (_test_neigh(g, i, j, TREE, LEFT) &&
        ((_neigh_count(g, i, j - 1, GRASS, false) + delta_grass_squares) == _neigh_size(g, i, j - 1, false)))
      return LOSING;
    if (_test_neigh(g, i, j, TREE, RIGHT) &&
        ((_neigh_count(g, i, j + 1, GRASS, false) + delta_grass_squares) == _neigh_size(g, i, j + 1, false)))
      return LOSING;
  }
  return REGULAR;  // regular move
}

static bool tent_adj_tent_all(cgame g) {
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (!r1_tent_adj_tent(g, i, j, game_get_square(g, i, j))) {
        return false;
      }
    }
  }
  return true;
}

static bool nb_tents_ok(cgame g) {
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_get_current_nb_tents_row(g, i) !=
          game_get_expected_nb_tents_row(g, i)) {
        return false;
      }
      if (game_get_current_nb_tents_col(g, j) !=
          game_get_expected_nb_tents_col(g, j)) {
        return false;
      }
    }
  }
  if (game_get_expected_nb_tents_all(g) < game_get_current_nb_tents_all(g)) {
    return false;
  }
  return true;
}

static bool tree_equal_tent(cgame g) {
  uint cpt_tree = 0;
  uint cpt_tent = 0;
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) == TREE) {
        cpt_tree++;
      }
      if (game_get_square(g, i, j) == TENT) {
        cpt_tent++;
      }
    }
  }
  if (cpt_tree != cpt_tent) {
    return false;
  }
  return true;
}

static bool tent_adj_tree_all(cgame g) {
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (!r3_tent_next_to_tree(g, i, j, game_get_square(g, i, j))) {
        return false;
      }
    }
  }
  return true;
}

bool game_is_over(cgame g) {
  assert(g);
  return tent_adj_tent_all(g) && nb_tents_ok(g) && tree_equal_tent(g) &&
         tent_adj_tree_all(g);
}

void game_fill_grass_row(game g, uint i) {
  assert(g);
  assert(i < g->nb_rows);
  assert(i >= 0);
  for (uint j = 0; j < g->nb_cols; j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      coup p0;
      p0.s = game_get_square(g, i, j);
      p0.i = i;
      p0.j = j;
      coup *data0 = (coup *)malloc(sizeof(coup));
      *data0 = p0;
      queue_push_head(g->pile1,
                      data0);  // on sauvegarde les données de la case (i,j)
      coup p1;
      p1.s = GRASS;
      p1.i = i;
      p1.j = j;
      coup *data = (coup *)malloc(sizeof(coup));
      *data = p1;
      queue_push_head(g->pile1, data);  // on sauvegarde le coup joué (GRASS)
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_fill_grass_col(game g, uint j) {
  assert(g);
  assert(j < g->nb_cols);
  assert(j >= 0);
  for (unsigned int i = 0; i < g->nb_rows; i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      coup p0;
      p0.s = game_get_square(g, i, j);
      p0.i = i;
      p0.j = j;
      coup *data0 = (coup *)malloc(sizeof(coup));
      *data0 = p0;
      queue_push_head(g->pile1,
                      data0);  // on sauvegarde les données de la case (i,j)
      coup p1;
      p1.s = GRASS;
      p1.i = i;
      p1.j = j;
      coup *data = (coup *)malloc(sizeof(coup));
      *data = p1;
      queue_push_head(g->pile1, data);  // on sauvegarde le coup joué (GRASS)
      game_set_square(g, i, j, GRASS);
    }
  }
}

void game_restart(game g) {
  assert(g);
  for (unsigned int i = 0; i < g->nb_rows; i++) {
    for (unsigned int j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) != TREE) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
  if (!queue_is_empty(g->pile1)) {
    queue_clear_full(g->pile1, &free);  // on vide la pile1
  }

  if (!queue_is_empty(g->pile2)) {
    queue_clear_full(g->pile2, &free);  // on vide la pile2
  }
}