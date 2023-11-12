#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int c;

    while ((c = getopt(argc, argv, "ab:")) != -1) {
        switch (c) {
        case 'a':
            printf("Option a\n");
            break;
        case 'b':
            printf("Option b with value %s\n", optarg);
            break;
        default:
            abort(); //это грех
        }
    }
    printf("optind = %d, argc = %d\n", optind, argc);

    for (int index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
  
    return 0;
}
