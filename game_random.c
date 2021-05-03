/**
 * @file game_random.c
 * @brief Generate a random game, print it and optionally save it.
 *
 **/

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_tools.h"
#include "game_aux.h"

/* command usage */
void usage(int argc, char *argv[]) {
  printf("Usage %s <nb_rows> <nb_cols> <wrapping> <diagadj> <nb_trees> <with_solution> [<filename>] \n", argv[0]);
  printf("Example: %s 4 4 0 0 2 1 random.tnt\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* main routine */
int main(int argc, char *argv[]) {
  if (argc < 7 || argc > 8) usage(argc, argv);

    game g = game_random(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4],atoi(argv[5]),argv[6]);
    
  while (game_is_over(g) == false) {
    game_print(g);
    printf("> ? [h for help]\n");
    char command;
    uint row, column;
    scanf(" %c", &command);
    if (command == 'h') {
      printf("> action: help \n");
      printf("- press 't <i> <j>' to put a tent at square (i,j) \n");
      printf("- press 'g <i> <j>' to put a grass at square (i,j) \n");
      printf("- press 'e <i> <j>' to erase square (i,j) \n");
      printf("- press 'z' to undo the last move \n");
      printf("- press 'y' to redo the last move \n");
      printf("- press 'r' to restart \n");
      printf("- press 'q' to quit \n");
      printf("- press 's' to resolve the game \n");
      game_print(g);
    }
    if (command == 'r') {
      game_restart(g);
      game_print(g);
      printf("> action: restart\n");
    }
    if (command == 's') {
      if (game_solve(g) == true) {
        break;
      } else {
        printf("> there are no solution");
      }
    }
    if (command == 'q') {
      game_delete(g);
      printf("> action: quit\nWhat a shame, you gave up :-(\n");
      return EXIT_SUCCESS;
    }
    if (command == 'z') {
      game_undo(g);
    }
    if (command == 'y') {
      game_redo(g);
    }
    scanf("%d %d", &row, &column);
    if (command == 't') {
      if (game_check_move(g, row, column, TENT) == REGULAR) {
        game_play_move(g, row, column, TENT);
        printf("> action: play move 't' into square (%d,%d)\n", row, column);
      }
      if (game_check_move(g, row, column, TENT) == LOSING) {
        game_play_move(g, row, column, TENT);
        printf("Warning: losing move on square (%d,%d)!\n", row, column);
      }
      if (game_check_move(g, row, column, TENT) == ILLEGAL) {
        printf("Warning: you can't put a tent on a tree (%d,%d)!\n", row,
               column);
      }
    }
    if (command == 'g') {
      if (game_check_move(g, row, column, GRASS) == REGULAR) {
        game_play_move(g, row, column, GRASS);
        printf("> action: play move 'g' into square (%d,%d)\n", row, column);
      }
      if (game_check_move(g, row, column, GRASS) == LOSING) {
        game_play_move(g, row, column, GRASS);
        printf("Warning: losing move on square (%d,%d)!\n", row, column);
      }
      if (game_check_move(g, row, column, GRASS) == ILLEGAL) {
        printf("Warning: you can't put a grass on a tree (%d,%d)!\n", row,
               column);
      }
    }
    if (command == 'e') {
      if (game_check_move(g, row, column, EMPTY) == REGULAR) {
        game_play_move(g, row, column, EMPTY);
        printf("> action: play move 'e' into square (%d,%d)\n", row, column);
      }
      if (game_check_move(g, row, column, EMPTY) == ILLEGAL) {
        printf("Warning: you can't remove a tree (%d,%d)!\n", row, column);
      }
    }
  }
    if (argc == 8){
        game_save(g,argv[7]);
    }
  game_print(g);
  return EXIT_SUCCESS;
}
