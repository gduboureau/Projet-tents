#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.c"
#include "game_aux.h"

int main(void) {
  game g = game_default();
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
      printf("- press 'r' to restart \n");
      printf("- press 'q' to quit \n");
      game_print(g);
    }
    if (command == 'r') {
      game_restart(g);
      game_print(g);
      printf("> action: restart\n");
    }
    if (command == 'q') {
      game_delete(g);
      printf("> action: quit\nWhat a shame, you gave up :-(\n");
      return EXIT_SUCCESS;
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
  game_print(g);
  printf("Congratulations ! You win :-)\n");
  return EXIT_SUCCESS;
}
