#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef unsigned int uint;

#define DEFAULT_SIZE 8

typedef enum {
  EMPTY = 0, /**< empty square */
  TREE = 1,  /**< tree square */
  TENT = 2,  /**< tent square */
  GRASS = 3, /**< grass square */
} square;

enum {
  ILLEGAL = 0, /**< illegal move */
  REGULAR = 1, /**< regular move */
  LOSING = 2,  /**< losing move */
};

typedef struct game_s *game;

typedef const struct game_s *cgame;

game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col){  
    return 0;
}

game game_new_empty(void){
    return 0;
}

game game_copy(cgame g){
    return 0;
}

bool game_equal(cgame g1, cgame g2){
    return 0;
}

void game_delete(game g){
    return ;
}