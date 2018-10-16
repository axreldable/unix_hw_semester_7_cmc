#include "library.h"

#include <stdio.h>

void cat(FILE * f) {
    int c;
    while ((c = fgetc(f)) != EOF) {
//        fputc(c, stdout);
        putchar(c);
    }
}

int main(int argc, char ** argv) {
    FILE *f;
    printf(argv[1]);
//    f = fopen(argv[1], "r");
}