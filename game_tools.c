#include "game_tools.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_ext.h"

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