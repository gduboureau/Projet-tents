#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "game_aux.h"
typedef unsigned int uint;


/* ********** TEST GAME GET CURRENT NB TENTS ROW ********** */

bool test__game_get_current_nb_tents_row(void){
    game g = game_default();
    for (uint i=0; i<DEFAULT_SIZE; i++){
        if (game_get_current_nb_tents_row(g, i) > game_get_expected_nb_tents_row(g,i)){
            fprintf(stderr,"Error: current tents row is not expected (row %d)!\n", i);
            game_delete(g);
            return false;
        }
    }
    game_delete(g);
    return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS COL ********** */

bool test__game_get_current_nb_tents_col(void){
    game g = game_default();
    for (uint j=0; j<DEFAULT_SIZE; j++){
        if (game_get_current_nb_tents_col(g, j) > game_get_expected_nb_tents_col(g,j)){
            fprintf(stderr,"Error: current tents col is not expected (col %d)!\n", j);
            game_delete(g);
            return false;
        }
    }
    game_delete(g);
    return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS ALL ********** */

bool test__game_get_current_nb_tents_all(void){
    game g = game_default();
    uint c = 0;
    for (uint v=0; v<DEFAULT_SIZE; v++){
        c+=game_get_current_nb_tents_col(g, v);
    }
    if (game_get_current_nb_tents_all(g) != c){
        fprintf(stderr,"Error: number of all current tents is incorrect!\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** GAME PLAY MOVE ********** */

bool test__game_play_move(void){
    game g = game_default();
    for (uint i=0; i<DEFAULT_SIZE; i++){
        for (uint j=0; j<DEFAULT_SIZE; j++){
            if (game_get_square(g,i,j)==TREE && ((game_play_move(g, i, j,TENT) || game_play_move(g, i, j,GRASS) || game_play_move(g, i, j,EMPTY)){
                fprintf(stderr,"Error: illegal move!\n");
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

/* ********** GAME CHECK MOVE ********** */

bool test__game_check_move(void){
    game g = game_default();
    for (uint i=0; i<DEFAULT_SIZE; i++){
        for(uint j=0; j<DEFAULT_SIZE; j++){
            if (game_check_move(g, i, j, game_get_square (g,i,j))!=REGULAR){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be REGULAR!\n", i, j);
                game_delete(g);
                return false;
            } 
        }
    }
    game_delete(g);
    return true;
}

/* ********** GAME IS OVER ********** */

bool test__game_is_over(void){
    game g = game_default();
    game g1 = game_default_solution();
    if (game_is_over(g)==true && game_equal(g,g1)==false){
        fprintf(stderr,"Error: the game over is different of game solution!\n");
        game_delete(g);
        game_delete(g1);
        return false;
    }
    if (game_is_over(g)==false){
        fprintf(stderr,"Error: the game over is different of game solution!\n");
        game_delete(g);
        game_delete(g1);
        return false;
    }
    game_delete(g);
    game_delete(g1);
    return true;
}


/* ********** USAGE ********** */

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char *argv[]){
    if (argc == 1){
        usage(argc, argv);
    }

    fprintf(stderr, "=> Start test \"%s\"\n enter \"echo $?\" to display if the test is successful.\n", argv[1]);
    bool ok = false;
    if (strcmp("game_get_current_nb_tents_row", argv[1]) == 0)
        ok = test__game_get_current_nb_tents_row();
    else if (strcmp("game_get_current_nb_tents_col", argv[1]) == 0)
        ok = test__game_get_current_nb_tents_col();
    else if (strcmp("game_get_current_nb_tents_all", argv[1]) == 0)
        ok = test__game_get_current_nb_tents_all();
    else if (strcmp("game_play_move", argv[1]) == 0)
        ok = test__game_play_move();
    else if (strcmp("game_check_move", argv[1]) == 0)
        ok = test__game_check_move();
    else if (strcmp("game_is_over", argv[1]) == 0)
        ok = test__game_is_over();
    else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
    }
    
    // print test result
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
