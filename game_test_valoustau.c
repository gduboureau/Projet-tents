#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "game_aux.h"
typedef unsigned int uint;


/* ********** TEST GAME GET CURRENT NB TENTS ROW ********** */

bool test_game_get_current_nb_tents_row(void){
    game g = game_default();
    game_play_move(g,0,0,TENT);
    if(game_get_current_nb_tents_row(g,0)!= 1){
        game_delete(g);
        return false;
    }
    
    game_delete(g);
    return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS COL ********** */

bool test_game_get_current_nb_tents_col(void){
    game g = game_default();
    game_play_move(g,0,3,TENT);
    if(game_get_current_nb_tents_col(g,3) != 1){
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** TEST GAME GET CURRENT NB TENTS ALL ********** */

bool test_game_get_current_nb_tents_all(void){
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

bool test_game_play_move(void){
    game g = game_default();
    for (uint i=0; i<DEFAULT_SIZE; i++){
        for (uint j=0; j<DEFAULT_SIZE; j++){
            if (game_get_square(g,i,j)==TREE && (game_check_move(g,i,j,EMPTY)!=ILLEGAL || game_check_move(g,i,j,GRASS)!=ILLEGAL || game_check_move(g,i,j,TENT)!=ILLEGAL || game_check_move(g,i,j,TREE)!=ILLEGAL)){
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

bool test_game_check_move(void){
    game g = game_default();
    for (uint i=0; i<DEFAULT_SIZE; i++){
        for(uint j=0; j<DEFAULT_SIZE; j++){
            //ILLEGAL MOVE
            if ((game_get_square(g,i,j)==TREE && (game_check_move(g,i,j,EMPTY)!=ILLEGAL || game_check_move(g,i,j,GRASS)!=ILLEGAL || game_check_move(g,i,j,TENT)!=ILLEGAL)) || game_check_move(g,i,j,TREE)!=ILLEGAL){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be ILLEGAL!\n", i, j);
                game_delete(g);
                return false;
            }
            //LOSING MOVE
            if (j!=0 && (game_get_square(g,i,j-1)==TENT && game_check_move(g,i,j,TENT)!=LOSING)){  //tent au dessus d'une autre
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
            if (j!=DEFAULT_SIZE-1 && (game_get_square(g,i,j+1)==TENT && game_check_move(g,i,j,TENT)!=LOSING)){ //tent en dessous d'une autre
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
            if (i!=0 && (game_get_square(g,i-1,j)==TENT && game_check_move(g,i,j,TENT)!=LOSING)){ //tent a gauche d'une autre
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
            if (i!=DEFAULT_SIZE-1 && (game_get_square(g,i+1,j)==TENT && game_check_move(g,i,j,TENT)!=LOSING)){ // tent a droite d'une autre
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }

            if(game_get_square(g, i, 0)==EMPTY && game_check_move(g, i, 0, GRASS) != LOSING){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
            
            if(game_get_expected_nb_tents_row(g, i)==0 && game_check_move(g, i, j, TENT) != LOSING && game_get_square(g,i,j)==EMPTY){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }

            if(game_get_expected_nb_tents_col(g, j)==0 && game_check_move(g, i, j, TENT) != LOSING && game_get_square(g,i,j)==EMPTY){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
            
            
            if(game_get_expected_nb_tents_row(g, i)>0 && (game_get_current_nb_tents_row(g, i) >= game_get_expected_nb_tents_row(g, i)) && game_check_move(g, i, j, TENT) != LOSING && game_get_square(g,i,j)!=TREE){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }

            if(game_get_expected_nb_tents_col(g, j)>0 &&(game_get_current_nb_tents_col(g, j) >= game_get_expected_nb_tents_col(g, j)) && game_check_move(g, i, j, TENT) != LOSING && game_get_square(g,i,j)!=TREE){
                fprintf(stderr,"Error: the game move at position (%d,%d) must be LOSING!\n", i, j);
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

/* ********** GAME IS OVER ********** */

bool test_game_is_over(void){
    game g = game_default();
    game g1 = game_default_solution();
    if (game_is_over(g)==true && game_equal(g,g1)==false){
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
        ok = test_game_get_current_nb_tents_row();
    else if (strcmp("game_get_current_nb_tents_col", argv[1]) == 0)
        ok = test_game_get_current_nb_tents_col();
    else if (strcmp("game_get_current_nb_tents_all", argv[1]) == 0)
        ok = test_game_get_current_nb_tents_all();
    else if (strcmp("game_play_move", argv[1]) == 0)
        ok = test_game_play_move();
    else if (strcmp("game_check_move", argv[1]) == 0)
        ok = test_game_check_move();
    else if (strcmp("game_is_over", argv[1]) == 0)
        ok = test_game_is_over();
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
