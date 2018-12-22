#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

int strings(char *name, size_t start_size, int length) {
    FILE *f;
    f = fopen(name, "r");
    if (f == NULL) {
        perror(name);
        return 0;
    }

    char *str;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * start_size);
    if (!str) {
        perror("not enough memory");
    }
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\t' || (ch >= 32 && ch < 127)) {
//            printf("!%d", ch);
            str[len++] = ch;
            if (len == start_size) {
                str = realloc(str, sizeof(char) * (start_size += 16));
                if (!str) {
                    perror("not enough memory");
                }
            }
            if (ch == '\n') {
                str[len++] = '\0';
                if (len >= length + 2) {
                    printf("%s", str);
                }
                len = 0;
                str = realloc(NULL, sizeof(char) * start_size);
                if (!str) {
                    perror("not enough memory");
                }
            }
        } else {
            if (len >= length) {
                str[len++] = '\n';
                str[len++] = '\0';
                printf("%s", str);
                len = 0;
                str = realloc(NULL, sizeof(char) * start_size);
                if (!str) {
                    perror("not enough memory");
                }
            } else {
                len = 0;
            }
        }

    }

    return 0;
}

int strings_std(size_t start_size, int length) {
    char *str;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * start_size);
    if (!str) {
        perror("not enough memory");
    }
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\t' || (ch >= 32 && ch < 127)) {
//            printf("!%d", ch);
            str[len++] = ch;
            if (len == start_size) {
                str = realloc(str, sizeof(char) * (start_size += 16));
                if (!str) {
                    perror("not enough memory");
                }
            }
            if (ch == '\n') {
                str[len++] = '\0';
                if (len >= length + 2) {
                    printf("%s", str);
                }
                len = 0;
                str = realloc(NULL, sizeof(char) * start_size);
                if (!str) {
                    perror("not enough memory");
                }
            }
        } else {
            if (len >= length) {
                str[len++] = '\n';
                str[len++] = '\0';
                printf("%s", str);
                len = 0;
                str = realloc(NULL, sizeof(char) * start_size);
                if (!str) {
                    perror("not enough memory");
                }
            } else {
                len = 0;
            }
        }

    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        strings_std(10, 4);
    }  else {
        for (int i = 1; i < argc; i++) {
            strings(argv[i], 10, 4);
        }
    }
}
