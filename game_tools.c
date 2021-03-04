#include "game_tools.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_ext.h"
#include "queue.h"

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

game game_load(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  uint nb_lignes, nb_col, wrap, diag;
  fscanf(f, "%u %u %u %u", &nb_lignes, &nb_col, &wrap, &diag);
  uint tent_ligne[nb_lignes];
  uint tent_col[nb_col];
  square squares[nb_lignes * nb_col];
  char tab[nb_lignes * nb_col];
  for (uint i = 0; i < nb_lignes; i++) {
    fscanf(f, "%d ", &tent_ligne[i]);
  }
  for (uint j = 0; j < nb_col; j++) {
    fscanf(f, "%d ", &tent_col[j]);
  }
  fseek(f, 10 + (nb_col * 2) + (nb_lignes * 2), 0);
  for (uint z = 0; z < nb_lignes * nb_col; z++) {
    fscanf(f, "%c", &tab[z]);
    if (tab[z] == ' ') {
      squares[z] = EMPTY;
    } else if (tab[z] == '-') {
      squares[z] = GRASS;
    } else if (tab[z] == '*') {
      squares[z] = TENT;
    } else if (tab[z] == 'x') {
      squares[z] = TREE;
    } else if (z < nb_lignes * nb_col) {
      z--;
    }
  }
  fclose(f);
  game g = game_new_ext(nb_lignes, nb_col, squares, tent_ligne, tent_col, wrap,
                        diag);
  return g;
}

void game_save(cgame g, char *filename) {
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%u %u %u %u\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_is_diagadj(g));
  for (uint i = 0; i < game_nb_rows(g); i++) {
    fprintf(f, "%u ", game_get_expected_nb_tents_row(g, i));
  }
  fprintf(f, "\n");
  for (uint i = 0; i < game_nb_cols(g); i++) {
    fprintf(f, "%u ", game_get_expected_nb_tents_col(g, i));
  }
  fprintf(f, "\n");
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        fprintf(f, " ");
      }
      if (game_get_square(g, i, j) == TREE) {
        fprintf(f, "x");
      }
      if (game_get_square(g, i, j) == TENT) {
        fprintf(f, "*");
      }
      if (game_get_square(g, i, j) == GRASS) {
        fprintf(f, "-");
      }
    }
    fprintf(f, "\n");
  }
  fclose(f);
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

// dire si il existe une solution du jeu en param. et
// modifier le jeu si il y en a une

static bool game_s1(game g) {
  game_restart(g);
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TREE) {
        for (int k = -1; k < 2; k++) {
          for (int l = -1; l < 2; l++) {
            if (correct_next_coor(g, make_coor(i, j),
                                  coor_to_dir(make_coor(k, l)))) {
              coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
              if (game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
                  REGULAR) {
                game_set_square(g, coo_next.ligne, coo_next.colonne, TENT);
              }
            }
          }
        }
      }
    }
  }
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_set_square(g, i, j, GRASS);
      }
    }
  }
  return game_is_over(g);
}

static bool game_s2(game g) {
  game_restart(g);
  for (unsigned int i = game_nb_rows(g) - 1; i >= 1; i--) {
    for (unsigned int j = game_nb_cols(g) - 1; j >= 1; j--) {
      if (game_get_square(g, i, j) == TREE) {
        for (int k = -1; k < 2; k++) {
          for (int l = -1; l < 2; l++) {
            if (correct_next_coor(g, make_coor(i, j),
                                  coor_to_dir(make_coor(k, l)))) {
              coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
              if (game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
                  REGULAR) {
                game_set_square(g, coo_next.ligne, coo_next.colonne, TENT);
              }
            }
          }
        }
      }
    }
  }
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_set_square(g, i, j, GRASS);
      }
    }
  }
  return game_is_over(g);
}

static bool game_s3(game g) {
  game_restart(g);
  for (unsigned int i = game_nb_rows(g) - 1; i >= 1; i--) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TREE) {
        for (int k = -1; k < 2; k++) {
          for (int l = -1; l < 2; l++) {
            if (correct_next_coor(g, make_coor(i, j),
                                  coor_to_dir(make_coor(k, l)))) {
              coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
              if (game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
                  REGULAR) {
                game_set_square(g, coo_next.ligne, coo_next.colonne, TENT);
              }
            }
          }
        }
      }
    }
  }
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_set_square(g, i, j, GRASS);
      }
    }
  }
  return game_is_over(g);
}

static bool game_s4(game g) {
  game_restart(g);
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = game_nb_cols(g) - 1; j >= 1; j--) {
      if (game_get_square(g, i, j) == TREE) {
        for (int k = -1; k < 2; k++) {
          for (int l = -1; l < 2; l++) {
            if (correct_next_coor(g, make_coor(i, j),
                                  coor_to_dir(make_coor(k, l)))) {
              coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
              if (game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
                  REGULAR) {
                game_set_square(g, coo_next.ligne, coo_next.colonne, TENT);
              }
            }
          }
        }
      }
    }
  }
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_set_square(g, i, j, GRASS);
      }
    }
  }
  return game_is_over(g);
}

bool game_solve(game g) {
  return game_s1(g) || game_s2(g) || game_s3(g) || game_s4(g);
}

uint game_nb_solutions(game g) {
  uint cpt = 0;
  game g1 = game_copy(g);
  game g2 = game_copy(g);
  game g3 = game_copy(g);
  bool s1 = game_s1(g);
  bool s2 = game_s2(g1);
  bool s3 = game_s3(g2);
  bool s4 = game_s4(g3);
  if (s1 == true && !game_equal(g, g1) && !game_equal(g, g2) &&
      !game_equal(g, g3)) {
    cpt += 1;
  }
  if (s2 == true && !game_equal(g1, g) && !game_equal(g1, g2) &&
      !game_equal(g1, g3)) {
    cpt += 1;
  }
  if (s3 == true && !game_equal(g2, g1) && !game_equal(g2, g) &&
      !game_equal(g2, g3)) {
    cpt += 1;
  }
  if (s4 == true && !game_equal(g3, g1) && !game_equal(g3, g2) &&
      !game_equal(g, g3)) {
    cpt += 1;
  }
  return cpt;
}

// bool game_solve(game g) {
//   for (unsigned int i = 0; i < game_nb_rows(g); i++) {
//   for (unsigned int j = 0; j < game_nb_rows(g); j++) {
//     if (game_get_square(g,i,j)==TREE){
//       for (int k = -1; k < 2; k++) {
//         for (int l = -1; l < 2; l++) {
//           if (correct_next_coor(g, make_coor(i, j), coor_to_dir(make_coor(k,
//           l)))) {
//             coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
//               if (game_check_move(g, coo_next.ligne, coo_next.colonne, TENT)
//               == REGULAR) {
//                    game_set_square(g, coo_next.ligne, coo_next.colonne,
//                    TENT);
//               }
//             }
//           }
//         }
//       }
//     }
//   }
//   for (unsigned int i = 0; i < game_nb_rows(g); i++) {
//     for (unsigned int j = 0; j < game_nb_cols(g); j++) {
//       if(game_get_square(g,i,j)==EMPTY){
//         game_set_square(g, i, j, GRASS);
//       }
//     }
//   }
//   return game_is_over(g);
// }

// static uint nb_tentes_posables(game g, uint i,uint j){
//   uint cpt;
// for (int k = -1; k < 2; k++) {
//   for (int l = -1; l < 2; l++) {
//     if (correct_next_coor(g, make_coor(i, j), coor_to_dir(make_coor(k, l))))
//     {
//       coor coo_next = next_coor(g, make_coor(i, j), make_coor(k, l));
//       if (g->diagadj == false) {
//           if ((i != 0 ||
//                j != 0) &&  // test pour ne pas parcourir la case (x,y)
//               game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
//               REGULAR) {
//             cpt += 1;
//           }
//       } else {
//         if ((i == 0 || j == 0) &&
//             (i + j != 0) &&  // test pour ne pas parcourir la case (x,y) et
//                               // les diagonales
//             game_check_move(g, coo_next.ligne, coo_next.colonne, TENT) ==
//             REGULAR) {
//           cpt += 1;
//         }
//       }
//     }
//   }
// }
//   return cpt;
// }
