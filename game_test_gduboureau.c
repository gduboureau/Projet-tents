#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "game_aux.h"


void usage (int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname> (trop ou pas assez d'arguments)\n", argv[0]);
}


/* *********************************************************** */

bool test_game_fill_grass_row(){
    game g = game_default();
    for (unsigned int i; i<DEFAULT_SIZE; i++){
        for(unsigned int j = 0 ; j<DEFAULT_SIZE; j++){
            if (game_check_move(g,i,j,TREE) == ILLEGAL){
                return false;
            }
        }
    }
    return true;
}

/* *********************************************************** */

bool test_game_restart(){
    game g = game_default();
    if (g==NULL){
        game_delete(g);
        return false;
    }
    for (unsigned int i = 0; i<DEFAULT_SIZE; i++){
        for (unsigned int j = 0; j<DEFAULT_SIZE; j++){
            if (game_get_square(g,i,j) == TENT || game_get_square(g,i,j) == GRASS){
                game_delete(g);
                return false;
            }
        }
    }
    if (game_is_over(g) == true){
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* *********************************************************** */

bool test_game_print(){
    game g = game_default();
    game_print(g);
    game_delete(g);
    return true;
}

/* *********************************************************** */

int main (int argc, char* argv[]){
    if (argc == 1){
        usage(argc, argv);
    }
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("game_restart",argv[1]) == 0){
        ok = test_game_restart();
    }else{
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (ok) {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }else{
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}