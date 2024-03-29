#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.c"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* *********************************************************** */

bool test_game_load(void) {
  game g = game_default();
  game_save(g, "fichier");
  game g0 = game_load("fichier");
  if (!game_equal(g, g0)) {
    game_delete(g0);
    game_delete(g);
    return false;
  }
  game g1 = game_new_empty_ext(4, 2, false, false);
  game_save(g1, "fichier2");
  game g2 = game_load("fichier2");
  if ((game_nb_rows(g1) != game_nb_rows(g2)) ||
      (game_nb_cols(g1) != game_nb_cols(g2))) {
    game_delete(g0);
    game_delete(g2);
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g0);
  game_delete(g2);
  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_save(void) {
  game g = game_default();
  game_save(g, "fichier");
  game l = game_load("fichier");
  game g1 = game_default();
  game_save(g1, "fichier2");
  game l1 = game_load("fichier2");
  if (!game_equal(l, l1)) {
    game_delete(g);
    game_delete(g1);
    game_delete(l);
    game_delete(l1);
    return false;
  }
  game_set_square(g1, 0, 0, TENT);
  game_save(g1, "fichier2");
  game l2 = game_load("fichier2");
  if (game_equal(l1, l2)) {
    game_delete(g);
    game_delete(g1);
    game_delete(l);
    game_delete(l1);
    game_delete(l2);
    return false;
  }
  game g2 = game_new_empty_ext(4, 2, false, false);
  game_save(g2, "fichier3");
  game l3 = game_load("fichier3");
  if ((game_nb_rows(g2) != game_nb_rows(l3)) ||
      (game_nb_cols(g2) != game_nb_cols(l3))) {
    game_delete(g);
    game_delete(g1);
    game_delete(g2);
    game_delete(l);
    game_delete(l1);
    game_delete(l2);
    game_delete(l3);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(l);
  game_delete(l1);
  game_delete(l2);
  game_delete(l3);
  return true;
}

/* *********************************************************** */
uint tentes_lig[] = {3, 0, 4, 0, 4, 0, 1, 0};
uint tentes_col[] = {4, 0, 1, 2, 1, 1, 2, 1};

square squares[] = {
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
    EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
    TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
    TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

/* *********************************************************** */

bool test_game_new_ext(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);

  for (uint i = 0; i < g->nb_rows; i++) {
    if (g->nb_tents_row[i] > g->nb_rows) {
      fprintf(stderr, "Error : the game is not correct!\n");
      game_delete(g);
      return false;
    }
  }

  for (uint k = 0; k < g->nb_cols; k++) {
    if (g->nb_tents_col[k] > g->nb_cols) {
      fprintf(stderr, "Error : the game is not correct!\n");
      game_delete(g);
      return false;
    }
  }

  for (uint j = 0; j < g->nb_rows * g->nb_cols; j++) {
    if (g->squares[j] != EMPTY && g->squares[j] != TREE) {
      fprintf(stderr, "Error : the game is not correct!\n");
      game_delete(g);
      return false;
    }
  }

  if (g->wrapping == true || g->diagadj == true) {
    game_delete(g);
    return false;
  }

  g->diagadj = true;
  g->wrapping = true;

  if (g->wrapping == false || g->diagadj == false) {
    game_delete(g);
    return false;
  }
  if (game_nb_cols(g) != g->nb_cols || game_nb_rows(g) != g->nb_rows) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_new_empty_ext(void) {
  game g = game_new_empty_ext(8, 8, false, false);
  for (uint i = 0; i < g->nb_rows; i++) {
    if (g->nb_tents_row[i] != 0) {
      fprintf(stderr, "Error : the game is not empty!\n");
      game_delete(g);
      return false;
    }
  }
  for (uint j = 0; j < g->nb_cols; j++) {
    if (g->nb_tents_col[j] != 0) {
      fprintf(stderr, "Error : the game is not empty!\n");
      game_delete(g);
      return false;
    }
  }

  for (uint x = 0; x < g->nb_rows * g->nb_cols; x++) {
    if (g->squares[x] != EMPTY) {
      fprintf(stderr, "Error : the game is not empty!\n");
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_play_move(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  uint c = 0;
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
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
  square squares2[] = {TENT, GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS,
                       TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
                       TENT, GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,
                       TREE, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS,
                       TENT, TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS,
                       TREE, GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS,
                       TENT, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
                       TREE, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

  game g = game_new_ext(8, 8, squares2, tentes_lig, tentes_col, false, false);
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

  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
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
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  for (uint i = 0; i < game_nb_rows(g); i++) {
    game_set_expected_nb_tents_row(g, i, game_get_expected_nb_tents_row(g, i));
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_set_expected_nb_tents_col(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  for (uint j = 0; j < game_nb_cols(g); j++) {
    game_set_expected_nb_tents_col(g, j, game_get_expected_nb_tents_col(g, j));
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_row(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  for (uint i = 0; i < game_nb_rows(g); i++) {
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
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  for (uint j = 0; j < game_nb_cols(g); j++) {
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
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  uint c = 0;
  for (uint j = 0; j < game_nb_cols(g); j++) {
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

  else if (strcmp("game_load", argv[1]) == 0) {
    ok = test_game_load();
  }

  else if (strcmp("game_save", argv[1]) == 0) {
    ok = test_game_save();
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