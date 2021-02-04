#include "game_tools.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_ext.h"

game game_load(char *filename) { return 0; }

void game_save(cgame g, char *filename) {
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%u %u %u %u \n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_is_diagadj(g));
  for (uint i = 0; i < game_nb_rows(g); i++) {
    fprintf(f, "%u ", game_get_expected_nb_tents_row(g, i));
    fprintf(f, "\n");
  }
  for (uint i = 0; i < game_nb_rows(g); i++) {
    fprintf(f, "%u ", game_get_expected_nb_tents_row(g, i));
    fprintf(f, "\n");
  }
  unsigned int j;
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        fprintf(f, "")  ;
      }
      if (game_get_square(g, i, j) == TREE) {
        fprintf(f, "x ");
      }
      if (game_get_square(g, i, j) == TENT) {
        fprintf(f, "* ");
      }
      if (game_get_square(g, i, j) == GRASS) {
        fprintf(f, "- ");
      }
    }
  }
  fprintf(f, "\n");
  fclose(f);
}