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

bool test_game_delete(void){
  game g = game_default();
  game_delete(g);
  return true;
}

/* *********************************************************** */

  bool test_game_copy(void){
  game g = game_default();
  game g1 = game_copy(g);
  if(game_equal(g,g1)==false){
    fprintf(stderr,"Error : g is different from the copied game!\n");
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

/* *********************************************************** */

bool test_game_equal(void){
  game g1 = game_default();
  game g2 = game_default();

  if(game_equal(g1, g2)==false){
      game_delete(g1);
      game_delete(g2);

      return false;
      }

game_set_square(g2, 0, 0, TENT);
game_set_expected_nb_tents_row(g2, 0, 2);
  
  if(game_equal(g1, g2)==true){
    game_delete(g1);
    game_delete(g2);

    return false;
  }

game_set_square(g1, 0, 0, TENT);
game_set_expected_nb_tents_row(g1, 0, 2);
  
  if(game_equal(g1, g2)==false){
    game_delete(g1);
    game_delete(g2);
    return false;
  }

g1 = game_new_empty();
g2 = game_new_empty();

  if(game_equal(g1, g2)==false){
    game_delete(g1);
    game_delete(g2);
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}
/* *********************************************************** */

bool test_game_new_empty(void){
  game g = game_new_empty();
  for(uint i=0;i<DEFAULT_SIZE;i++){
      if(game_get_expected_nb_tents_row(g,i)!=0){
        fprintf(stderr,"Error : the game is not empty!\n");
        game_delete(g);
        return false;
      }
  }
  for(uint j=0;j<DEFAULT_SIZE;j++){
    if(game_get_expected_nb_tents_row(g,j)!=0){
        fprintf(stderr,"Error : the game is not empty!\n");
        game_delete(g);
        return false;
      }
  }

  for(uint x=0;x<DEFAULT_SIZE;x++){
    for(uint y=0;y<DEFAULT_SIZE;y++){
      if(game_get_square(g,x,y)!=EMPTY){
        fprintf(stderr,"Error : the game is not empty!\n");
        game_delete(g);
        return false;        
      };
    }
  }
  game_delete(g);
  return true;
}
   
/* *********************************************************** */

bool test_game_new(){
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

  game g = game_new(squares,tentes_lig,tentes_col);
  game_delete(g);
  return true;
}

/* *********************************************************** */

bool test_game_set_square(){
  game g = game_new_empty();
  game_set_square(g, 0, 4, TREE);
  if(game_get_square(g,0,4)!= TREE){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* *********************************************************** */

int main(int argc, char *argv[]) {
  if (argc == 1){
    usage(argc, argv);
  }

   // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if(strcmp("game_delete", argv[1]) == 0){
    ok = test_game_delete();
  }
  else if(strcmp("game_copy", argv[1]) == 0){
    ok = test_game_copy();
  }
  else if(strcmp("game_equal", argv[1]) == 0){
    ok = test_game_equal();
  }
  else if(strcmp("game_new_empty", argv[1]) == 0){
    ok = test_game_new_empty();
  }
  else if(strcmp("game_new", argv[1]) == 0){
    ok = test_game_new();
  }
  else if(strcmp("game_set_square", argv[1]) == 0){
    ok = test_game_set_square();
  }
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