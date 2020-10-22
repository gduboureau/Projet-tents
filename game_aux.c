#include <stdlib.h>
#include <stdio.h>
#include "game_aux.h"
#include "game.h"

void game_print(cgame g){
    printf("   ");
    for (unsigned int y=0; y<DEFAULT_SIZE; y++){

            printf("%d", y);
        }
    printf("\n   ");
    for (unsigned int z=0; z<DEFAULT_SIZE; z++){
        printf("-");
        }
    unsigned int j;
    for (unsigned int i=0; i<DEFAULT_SIZE; i++){
        printf("\n%d |", i);
        for (j=0; j<DEFAULT_SIZE; j++){
            if (game_get_square(g, i, j)==EMPTY){
               printf(" ");
           }
            if (game_get_square(g, i, j)==TREE){
               printf("x");
            }
            if (game_get_square(g, i, j)==TENT){
               printf("*");
            }
            if (game_get_square(g, i, j)==GRASS){
               printf("-");
            }
        }
            if(j==DEFAULT_SIZE){
                uint lign=game_get_expected_nb_tents_row(g, i);
                printf("| %u", lign);
            }
    }

   printf("\n   ");
    
    for (unsigned int z=0; z<DEFAULT_SIZE; z++){
        printf("-");
        }
    printf("\n   ");
    for (unsigned int w=0; w<DEFAULT_SIZE; w++){
            uint colo=game_get_expected_nb_tents_col(g, w);
            printf("%u", colo);

        }
    printf("\n");
    
    
    return ;
}

game game_default(void){

    
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

    game g = game_new(squares, tentes_lig, tentes_col);


    return g;
}


game game_default_solution(void){
    
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
    game g = game_new(squares, tentes_lig, tentes_col);

    return g;
}