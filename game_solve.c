#include <stdio.h>
#include <stdlib.h>
#include "game.c"
#include "game.h"
#include "game_aux.c"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4){
        fprintf(stderr,"wrong number of parameters\n");
        exit(EXIT_FAILURE);
    }
    game g = game_load(argv[2]);    
    if( strcmp(argv[1], "-s") == 0) {
        if (game_solve(g)){
            if (argc == 4){
                game_save(g,argv[3]);
                printf("Solution dans le fichier de sortie\n");
            }else{
                game_print(g);
            }
        }else{
            fprintf(stderr,"pas de solution\n");
            exit(EXIT_FAILURE);
        }
    }else if( strcmp(argv[1], "-c") == 0) {
        printf("n'existe pas encore\n");
    }else{
        fprintf(stderr,"please enter s for solve the game or c to find the number of solution");
        exit(EXIT_FAILURE);
    }
    game_delete(g);
    return EXIT_SUCCESS;
}
