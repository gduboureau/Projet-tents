#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "game.h"
#include "game_aux.h"



void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* *********************************************************** */

bool test_game_get_square(void){
    game g = game_default();
    unsigned int i = 0;
    unsigned int j = 0;
    for(i; i<DEFAULT_SIZE; i++){
        for(j; j<DEFAULT_SIZE; j++){
            if(game_get_square(g, i, j) != TENT && game_get_square(g, i, j) != EMPTY && game_get_square(g, i, j) != GRASS && game_get_square(g, i, j) != TREE){
                fprintf(stderr, "Error : the square isn't valid!\n");
                return false;
            }
        }    
    }
    return true;
}

/* *********************************************************** */

//bool test_game_set_expected_nb_tents_row(void){
//}

/* *********************************************************** */

//bool test_game_set_expected_nb_tents_col(void){
//}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_row(void){
    game g = game_default();
    unsigned int i = 0;
    for(i; i<DEFAULT_SIZE; i++){
        if(game_get_expected_nb_tents_row(g, i)<0){
            fprintf(stderr, "Error : number of tents isn't valid!\n");
            return false;
        }
    }    
    return true;
}

/* *********************************************************** */

bool test_game_get_expected_nb_tents_col(void){
    game g = game_default();
    unsigned int j = 0;
    for(j; j<DEFAULT_SIZE; j++){
        if(game_get_expected_nb_tents_row(g, j)<0){
            fprintf(stderr, "Error : number of tents isn't valid!\n");
            return false;
        }
    }    
    return true;
}

/* *********************************************************** */

//bool test_game_get_expected_nb_tents_all(void){
//}

/* *********************************************************** */

int main(int argc, char *argv[]) {
  if (argc == 1){
    usage(argc, argv);
  }

   // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);

  bool ok = false;

  if(strcmp("game_get_square", argv[1]) == 0){
    ok = test_game_get_square();
  }
  /*
  else if(strcmp("game_set_expected_nb_tents_row", argv[1]) == 0){
    ok = test_game_set_expected_nb_tents_row();
  }
  else if(strcmp("game_set_expected_nb_tents_col", argv[1]) == 0){
    ok = test_game_set_expected_nb_tents_col();
  }
  */
  else if(strcmp("game_get_expected_nb_tents_row", argv[1]) == 0){
    ok = test_game_get_expected_nb_tents_row();
  }
  else if(strcmp("game_get_expected_nb_tents_col", argv[1]) == 0){
    ok = test_game_get_expected_nb_tents_col();
  }
  /*
  else if(strcmp("game_get_expected_nb_tents_all", argv[1]) == 0){
    ok = test_game_get_expected_nb_tents_all();
  }
  */
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
   if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } 
  else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}