#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage (char *nom){
    fprintf(stderr,"Usage: %s <testname> \n",nom);
    exit(EXIT_FAILURE);
}

int main(int argc,char *argv[]){
    if(argc!=2){
        usage(argv[0]);
    }
    if(strcmp("dummy",argv[1])==0){
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}