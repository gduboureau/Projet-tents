#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

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

typedef struct{
    square *squares;
    uint *nb_tents_row;
    uint *nb_tents_col;
} game_s ;

typedef const struct{
    square *squares;
    uint *nb_tents_row;
    uint *nb_tents_col;
} game_s ;

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

void game_set_square(game g, uint i, uint j, square s){
    return ;
}

square game_get_square(cgame g, uint i, uint j){
    return 0;
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents){
    return ;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents){
    return ;
}

uint game_get_expected_nb_tents_row(cgame g, uint i){
    return 0;
}

uint game_get_expected_nb_tents_col(cgame g, uint j){
    return 0;
}

uint game_get_expected_nb_tents_all(cgame g){
    return 0;
}

uint game_get_current_nb_tents_row(cgame g, uint i){
    return 0;
}

uint game_get_current_nb_tents_col(cgame g, uint j){
    return 0;
}

uint game_get_current_nb_tents_all(cgame g){
    return 0;
}

void game_play_move(game g, uint i, uint j, square s){
    return ;
}

int game_check_move(cgame g, uint i, uint j, square s){
    return 0;
}

bool game_is_over(cgame g){
    return 0;
}

void game_fill_grass_row(game g, uint i){
    return ;
}

void game_fill_grass_col(game g, uint j){
    return ;
}

void game_restart(game g){
    return ;
}