#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage (char *name){
    fprintf(stderr,"Usage: %s <testname> \n",name);
    exit(EXIT_FAILURE);
}

int main (int argc, char* argv[]){
    if(argc!=2){
        usage(argv[0]);
    }
    if(strcmp("dummy",argv[1])==0){
        return 0;
    }
    return 1;
}