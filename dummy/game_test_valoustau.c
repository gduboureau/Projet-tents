#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char *argv[]) {
    if (argc == 1) usage(argc, argv);

fprintf(stderr, "=> Start test \"%s\"\n enter \"echo $?\" to display if the test is successful.\n", argv[1]);
    if (strcmp("dummy", argv[1]) == 0)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}