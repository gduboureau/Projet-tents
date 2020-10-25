#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage (int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname> (trop ou pas assez d'arguments)\n", argv[0]);
}

int main (int argc, char* argv[]){
    if (argc == 2){
        if (strcmp(argv[1], "dummy") == 0){
            return 0;
        }
        return 1;
    }
    usage(argc,argv);
}