#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <memory.h>

char *read_file(char *name, size_t start_size) {
    FILE *in;
    in = fopen(name, "r");
    if (in == NULL) {
        perror(name);
        return 0;
    }

    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * start_size);
    if (!str) {
        perror("not enough memory");
    }
    while ((ch = fgetc(in)) != EOF) {
        str[len++] = ch;
        if (len == start_size) {
            str = realloc(str, sizeof(char) * (start_size += 16));
            if (!str) {
                perror("not enough memory");
            }
        }
    }
    str[len++] = '\0';

    return realloc(str, sizeof(char) * len);
}

char *replaceWord(const char *s, const char *oldW,
                  const char *newW) {
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }

    result = (char *) malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        } else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}


int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "USAGE: ./a.out pattern change file_list\n");
        exit(1);
    }

    char *pattern = argv[1];
    char *change = argv[2];

    for (int i = 3; i < argc; i++) {
        char *str = read_file(argv[i], 10);
        char *result = NULL;

//        printf("Old string: %sn", str);

        result = replaceWord(str, pattern, change);
        printf("%s", result);

        free(result);
    }
    return 0;
}
