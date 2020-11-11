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
    game g1 = game_default();
    for (unsigned int i = 0; i<DEFAULT_SIZE; i++){
        game_fill_grass_row(g,i);
    }
    for (unsigned int i = 0; i<DEFAULT_SIZE; i++){
        for (unsigned int j = 0; j<DEFAULT_SIZE; j++){
            if ((game_get_square(g1,i,j) == EMPTY && game_get_square(g,i,j) != GRASS) || (game_get_square(g1,i,j) != EMPTY && game_get_square(g,i,j) == GRASS)){
                game_delete(g);
                game_delete(g1);
                return false;
            }
        }
    }
    game_delete(g);
    game_delete(g1);
    return true;
}


/* *********************************************************** */

bool test_game_fill_grass_col(){
    game g = game_default();
    game g1 = game_default();
    for (unsigned int j = 0; j<DEFAULT_SIZE; j++){
        game_fill_grass_col(g,j);
    }
    for (unsigned int i = 0; i<DEFAULT_SIZE; i++){
        for (unsigned int j = 0; j<DEFAULT_SIZE; j++){
            if ((game_get_square(g1,i,j) == EMPTY && game_get_square(g,i,j) != GRASS) || (game_get_square(g1,i,j) != EMPTY && game_get_square(g,i,j) == GRASS)){
                game_delete(g);
                game_delete(g1);
                return false;
            }
        }
    }
    game_delete(g);
    game_delete(g1);
    return true;
}


/* *********************************************************** */

bool test_game_restart(){
    game g = game_default_solution();
    if (g==NULL){
        game_delete(g);
        return false;
    }
    game_restart(g);
    for (unsigned int i = 0; i<DEFAULT_SIZE; i++){
        for (unsigned int j = 0; j<DEFAULT_SIZE; j++){
            if (game_get_square(g,i,j) == TENT || game_get_square(g,i,j) == GRASS){
                game_delete(g);
                return false;
            }
        }
    }
    game g1 = game_default();
    if (game_equal(g,g1) == false){
        game_delete(g);
        game_delete(g1);
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

bool test_game_default(){
    game g = game_default();
    uint tentes_lig[] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint tentes_col[] = {4, 0, 1, 2, 1, 1, 2, 1};

    square squares[] = {EMPTY, EMPTY, EMPTY,EMPTY,TREE,TREE,EMPTY,EMPTY,
    TREE, EMPTY, EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,TREE,
    EMPTY, EMPTY, EMPTY,EMPTY,TREE,EMPTY,EMPTY,EMPTY,
    TREE, EMPTY, EMPTY,EMPTY,EMPTY,TREE,EMPTY,EMPTY,
    EMPTY, TREE, EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
    TREE, EMPTY, EMPTY,EMPTY,TREE,EMPTY,TREE,EMPTY,
    EMPTY, EMPTY, EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,
    TREE,EMPTY, EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY} ;

    game g1 = game_new(squares, tentes_lig, tentes_col);
    if (game_equal(g,g1) == false){
        game_delete(g);
        game_delete(g1);
        return false;
    }
    game_delete(g);
    game_delete(g1);
    return true;
}

/* *********************************************************** */
bool test_game_default_solution(){
    game g = game_default_solution();
    uint tentes_lig[] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint tentes_col[] = {4, 0, 1, 2, 1, 1, 2, 1};

    square squares[] = {TENT, GRASS, GRASS,TENT,TREE,TREE,TENT,GRASS,
    TREE, GRASS, GRASS,GRASS,GRASS,GRASS,GRASS,TREE,
    TENT, GRASS, GRASS,TENT,TREE,TENT,GRASS,TENT,
    TREE, GRASS, GRASS,GRASS,GRASS,TREE,GRASS,GRASS,
    TENT, TREE, TENT,GRASS,TENT,GRASS,TENT,GRASS,
    TREE, GRASS, GRASS,GRASS,TREE,GRASS,TREE,GRASS,
    TENT, GRASS, GRASS,GRASS,GRASS,GRASS,GRASS,GRASS,
    TREE,GRASS, GRASS,GRASS,GRASS,GRASS,GRASS,GRASS} ;

    game g1 = game_new(squares, tentes_lig, tentes_col);
    if (game_equal(g,g1) == false){
        game_delete(g);
        game_delete(g1);
        return false;
    }
    game_delete(g);
    game_delete(g1);
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
    }
    else if(strcmp("game_fill_grass_row", argv[1]) == 0){
        ok = test_game_fill_grass_row();
    }
    else if(strcmp("game_fill_grass_col", argv[1]) == 0){
        ok = test_game_fill_grass_col();
    }
    else if(strcmp("game_print", argv[1]) == 0){
        ok = test_game_print();
    }
    else if(strcmp("game_default", argv[1]) == 0){
        ok = test_game_default();
    }
    else if(strcmp("game_default_solution", argv[1]) == 0){
        ok = test_game_default_solution();
    }
    else{
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