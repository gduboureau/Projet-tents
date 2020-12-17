#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game.c"
#include "game_aux.h"

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* *********************************************************** */

bool test_game_new_ext(void){
  return true;
}



/* *********************************************************** */


bool test_game_new_empty_ext(void){
  game g = game_new_empty_ext(1, 1, false, false);
  for (uint o = 1; o < 10; o++) { /* nb rows de 1 à 9*/
    for (uint p = 1; p < 10; p++) { /* nb cols de 1 à 9 */
      for (uint q = 0; q < 2; q++) { /* wrapping true ou false*/
        for (uint r = 0; r < 2; r++) { /* diagadj true ou false*/
          g = game_new_empty_ext(o, p, q, r);
          for (uint i = 0; i < g->nb_rows; i++) {
            if (game_get_expected_nb_tents_row(g, i) != 0) {
              fprintf(stderr, "Error : the game is not empty!\n");
              game_delete(g);
              return false;
            }
          }
          for (uint j = 0; j < g->nb_cols; j++) {
            if (game_get_expected_nb_tents_row(g, j) != 0) {
              fprintf(stderr, "Error : the game is not empty!\n");
              game_delete(g);
              return false;
            }
          }

          for (uint x = 0; x < g->nb_rows; x++) {
            for (uint y = 0; y < g->nb_cols; y++) {
              if (game_get_square(g, x, y) != EMPTY) {
                fprintf(stderr, "Error : the game is not empty!\n");
                game_delete(g);
                return false;
              }
            }
          }
        }
      }
    }
  }
  game_delete(g);
  return true;
}


/* *********************************************************** */

bool test_game_play_move(void) {
  game g = game_default();
  uint c = 0;
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_play_move(g, i, j, TENT);
      }
      if (game_get_square(g, i, j) == EMPTY) {
        c++;
      }
    }
  }
  if (c != 0) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_get_square(void) {
  game g = game_default_solution();
  if (game_get_expected_nb_tents_row(g, 0) != 3) {
    game_delete(g);
    return false;
  }

  if (game_get_square(g, 0, 1) == TREE) {
    game_delete(g);
    return false;
  }

  if (game_get_square(g, 1, 0) == GRASS) {
    game_delete(g);
    return false;
  }

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != TENT &&
          game_get_square(g, i, j) != EMPTY &&
          game_get_square(g, i, j) != GRASS &&
          game_get_square(g, i, j) != TREE) {
        fprintf(stderr, "Error : the square isn't valid!\n");
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_set_expected_nb_tents_row(void) {
  game g = game_default();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    game_set_expected_nb_tents_row(g, i, game_get_expected_nb_tents_row(g, i));
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_set_expected_nb_tents_col(void) {
  game g = game_default();
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    game_set_expected_nb_tents_col(g, j, game_get_expected_nb_tents_col(g, j));
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_row(void) {
  game g = game_default();
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) < 0 ||
        game_get_expected_nb_tents_row(g, i) > 4) {
      fprintf(stderr, "Error : number of tents isn't valid!\n");
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_col(void) {
  game g = game_default();
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_expected_nb_tents_col(g, j) < 0 ||
        game_get_expected_nb_tents_col(g, j) > 4) {
      fprintf(stderr, "Error : number of tents isn't valid!\n");
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_all(void) {
  game g = game_default();
  uint c = 0;
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    c = c + game_get_expected_nb_tents_col(g, j);
  }
  if (game_get_expected_nb_tents_all(g) != c) {
    fprintf(stderr, "Error : number of tents isn't valid!\n");
    game_delete(g);
    return false;
  }
  if (game_get_expected_nb_tents_all(g) < 0 ||
      game_get_expected_nb_tents_all(g) > 12) {
    fprintf(stderr, "Error : number of tents isn't valid!\n");
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);

  bool ok = false;

  if (strcmp("game_get_square", argv[1]) == 0) {
    ok = test_game_get_square();
  }

  else if (strcmp("game_set_expected_nb_tents_row", argv[1]) == 0) {
    ok = test_game_set_expected_nb_tents_row();
  } else if (strcmp("game_set_expected_nb_tents_col", argv[1]) == 0) {
    ok = test_game_set_expected_nb_tents_col();
  }

  else if (strcmp("game_get_expected_nb_tents_row", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_row();
  } else if (strcmp("game_get_expected_nb_tents_col", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_col();
  }

  else if (strcmp("game_get_expected_nb_tents_all", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_all();
  }

  else if (strcmp("game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  }

  else if (strcmp("game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext();
  }

  else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  }

  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}