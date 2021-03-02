#include <stdio.h>
#include <stdlib.h>
#include "game.c"
#include "game.h"
#include "game_aux.c"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
    game g = game_load(argv[1]);
    if (game_solve(g)){
        game_print(g);
    }else{
        fprintf(stderr,"pas de solution");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
