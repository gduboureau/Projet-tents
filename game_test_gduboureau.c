#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <game.h>
#include <game_aux.h>

bool test_game_fill_grass_row(game g, uint i){
    test_game_fill_grass_row(g,i);
    square squares[]={};
    for (unsigned int i; i<(i+1); i++){
        for (unsigned int j = 0; j<DEFAULT_SIZE;j++){
            if (game_check_move(g, i, j, GRASS)==ILLEGAL){
                return false;
            }
        }
    }
    return true;
}

void usage (int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname> (trop ou pas assez d'arguments)\n", argv[0]);
}

int main (int argc, char* argv[]){
    game g=game_default();
    if (strcmp(argv[1], "test_game_fill_grass_row") == 0){
        test_game_fill_grass_row(g,argv[1]);
    }
}