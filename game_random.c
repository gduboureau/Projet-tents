/**
 * @file game_random.c
 * @brief Generate a random game, print it and optionally save it.
 *
 **/

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_tools.c"
#include "game_aux.c"

/**
 * @brief Generates a random game with the given attributes.
 * @details The function will try to generate a game with @p nb_rows rows and @p
 * nb_cols columns, and @p nb_trees trees.
 * @param nb_rows the number of rows of the expected game.
 * @param nb_cols the number of columns of the expected game.
 * @param diagadj game option.
 * @param wrapping game option.
 * @param nb_trees the number of trees in the expected game.
 * @param with_solution controls wether the returned game contains the solution
 * or not.
 * @return the new generated game
 */
game game_random(uint nb_rows, uint nb_cols, bool wrapping, bool diagadj, uint nb_trees, bool with_solution) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, diagadj);
  int num_tree = 0;
  while (num_tree < nb_trees) {
    uint i;
    uint j;
    uint itree, jtree;
    int direction, di, dj;
    bool pseudo_valid = false;
    do {
      pseudo_valid = true;
      i = rand() % nb_rows;
      j = rand() % nb_cols;
      direction = rand() % 4;
      int dx[] = {0, 0, 1, -1};
      int dy[] = {1, -1, 0, 0};
      di = dx[direction];
      dj = dy[direction];
      itree = i + di;
      jtree = j + dj;
      if (wrapping) {
        itree = (itree + nb_rows) % nb_rows;
        jtree = (jtree + nb_cols) % nb_cols;
      }
      pseudo_valid = pseudo_valid && (itree >= 0 && itree < nb_rows);
      pseudo_valid = pseudo_valid && (jtree >= 0 && jtree < nb_cols);
      pseudo_valid = pseudo_valid && game_get_square(g, i, j) == EMPTY;
      pseudo_valid = pseudo_valid && game_get_square(g, itree, jtree) == EMPTY;
    } while (!pseudo_valid);
    game_set_square(g, i, j, TENT);
    game_set_expected_nb_tents_row(g, i, game_get_expected_nb_tents_row(g, i) + 1);
    game_set_expected_nb_tents_col(g, j, game_get_expected_nb_tents_col(g, j) + 1);
    game_set_square(g, itree, jtree, TREE);
    if (game_is_over(g)) {
      num_tree++;
    } else {
      game_set_square(g, i, j, EMPTY);
      game_set_expected_nb_tents_row(g, i, game_get_expected_nb_tents_row(g, i) - 1);
      game_set_expected_nb_tents_col(g, j, game_get_expected_nb_tents_col(g, j) - 1);
      game_set_square(g, itree, jtree, EMPTY);
    }
  }
  if (!with_solution) game_restart(g);
  return g;
}

/* command usage */
void usage(int argc, char *argv[]) {
  printf("Usage %s <nb_rows> <nb_cols> <wrapping> <diagadj> <nb_trees> <with_solution> [<filename>] \n", argv[0]);
  printf("Example: %s 4 4 0 0 2 1 random.tnt\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* main routine */
int main(int argc, char *argv[]) {
  if (argc < 7 || argc > 8) usage(argc, argv);

  game g = game_random(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
  game_print(g);
  game_save(g,argv[6]);
  
  // TODO: implement this

  return EXIT_SUCCESS;
}

