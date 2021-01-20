#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.c"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
typedef unsigned int uint;

uint tentes_lig[] = {3, 1, 4, 0, 4, 0, 1, 1};
uint tentes_col[] = {4, 1, 1, 2, 1, 1, 2, 1};

square squares[] = {
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
    EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
    TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
    TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

square squares2[] = {
    TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS, GRASS,
    GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,  TREE,  TENT,
    GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, TENT,
    TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS, TREE,  GRASS, GRASS, GRASS,
    TREE,  GRASS, TREE,  GRASS, TENT,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
    GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

/* ********** TEST GAME GET CURRENT NB TENTS ROW ********** */

bool test_game_get_current_nb_tents_row(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  game g1 = game_new_ext(8, 8, squares2, tentes_lig, tentes_col, false, false);
  uint c = 0;
  for (uint i = 0; i < game_nb_rows(g1); i++) {
    c = c + game_get_current_nb_tents_row(g1, i);
    if (game_get_current_nb_tents_row(g, i) != 0) {
      game_delete(g);
      game_delete(g1);
      return false;
    }
  }
  if (c != 12) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS COL ********** */

bool test_game_get_current_nb_tents_col(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  game g1 = game_new_ext(8, 8, squares2, tentes_lig, tentes_col, false, false);
  uint c = 0;
  for (uint j = 0; j < game_nb_cols(g1); j++) {
    c = c + game_get_current_nb_tents_col(g1, j);
    if (game_get_current_nb_tents_col(g, j) != 0) {
      game_delete(g);
      game_delete(g1);
      return false;
    }
  }
  if (c != 12) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS ALL ********** */

bool test_game_get_current_nb_tents_all(void) {
  game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  uint c = 0;
  for (uint v = 0; v < game_nb_cols(g); v++) {
    c += game_get_current_nb_tents_col(g, v);
  }
  if (game_get_current_nb_tents_all(g) != c) {
    fprintf(stderr, "Error: number of all current tents is incorrect!\n");
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* ********** GAME PLAY MOVE ********** */

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

/* ********** GAME CHECK MOVE ********** */

bool test_game_check_move(void) {
  // DIAGADJ
  /*game g = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, true);
  game_set_square(g, 0, 0, TENT);
  if (game_check_move(g, 1, 1, TENT) == LOSING) {
    game_delete(g);
    return false;
  }
  game g1 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false, false);
  game_set_square(g1, 0, 0, TENT);
  if (game_check_move(g1, 1, 1, TENT) != LOSING) {
    game_delete(g);
    game_delete(g1);
    return false;
  }*/
  // WRAPPING
  // game g2 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, true, true);
  // game g3 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, true, false);

  // if (game_check_move(g3, 7, 7, TENT) == LOSING) {
  //   // game_delete(g);
  //   // game_delete(g1);
  //   game_delete(g2);
  //   game_delete(g3);
  //   return false;
  // }
  // game_set_square(g2, 4, 7, TENT);
  // if (game_check_move(g2, 4, 0, TENT) != LOSING) {
  //   // game_delete(g);
  //   // game_delete(g1);
  //   game_delete(g2);
  //   game_delete(g3);
  //   return false;
  // }
  // game_set_square(g3, 4, 7, TENT);
  // if (game_check_move(g3, 4, 0, TENT) != LOSING) {
  //   // game_delete(g);
  //   // game_delete(g1);
  //   game_delete(g2);
  //   game_delete(g3);
  //   return false;
  // }
  // game_set_square(g2, 7, 7, GRASS);
  // game_set_square(g2, 7, 1, GRASS);
  // game_set_square(g2, 6, 0, GRASS);
  // if (game_check_move(g2, 0, 0, GRASS) != LOSING) {
  //   // game_delete(g);
  //   // game_delete(g1);
  //   game_delete(g2);
  //   game_delete(g3);
  //   return false;
  // }
  // // game_delete(g);
  // // game_delete(g1);
  // game_delete(g2);
  // game_delete(g3);

  // // CHECK_MOVE
  // // r2_nb_tent_respecte
  // game g4 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false,
  // false); game_set_square(g4, 6, 7, TENT); if (game_check_move(g4, 7, 7,
  // TENT) != LOSING) {
  //   game_delete(g4);
  //   return false;
  // }

  // // r3_tent_next_to_tree
  // game g5 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false,
  // false); if (game_check_move(g5, 4, 6, TENT) == LOSING) {
  //   game_delete(g4);
  //   game_delete(g5);
  //   return false;
  // }
  // if (game_check_move(g5, 6, 2, TENT) != LOSING) {
  //   game_delete(g4);
  //   game_delete(g5);
  //   return false;
  // }

  // // r4_nb_tent_grass
  // game g6 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false,
  // false); if (game_check_move(g6, 0, 0, GRASS) != LOSING) {
  //   game_delete(g4);
  //   game_delete(g5);
  //   game_delete(g6);
  //   return false;
  // }

  // // r5_tree_entoure_grass
  // game g7 = game_new_ext(8, 8, squares, tentes_lig, tentes_col, false,
  // false); game_set_square(g7, 6, 0, GRASS); if (game_check_move(g7, 7, 1,
  // GRASS) != LOSING) {
  //   game_delete(g4);
  //   game_delete(g5);
  //   game_delete(g6);
  //   game_delete(g7);
  //   return false;
  // }

  // game_delete(g4);
  // game_delete(g5);
  // game_delete(g6);
  // game_delete(g7);
  return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }

  fprintf(stderr,
          "=> Start test \"%s\"\n enter \"echo $?\" to display if the test is "
          "successful.\n",
          argv[1]);
  bool ok = false;
  if (strcmp("game_get_current_nb_tents_row", argv[1]) == 0)
    ok = test_game_get_current_nb_tents_row();
  else if (strcmp("game_get_current_nb_tents_col", argv[1]) == 0)
    ok = test_game_get_current_nb_tents_col();
  else if (strcmp("game_get_current_nb_tents_all", argv[1]) == 0)
    ok = test_game_get_current_nb_tents_all();
  else if (strcmp("game_play_move", argv[1]) == 0)
    ok = test_game_play_move();
  else if (strcmp("game_check_move", argv[1]) == 0)
    ok = test_game_check_move();
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
