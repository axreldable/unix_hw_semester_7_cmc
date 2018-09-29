#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int words_compare(const void *a, const void *b) {
    const char *pa = *(const char **) a;
    const char *pb = *(const char **) b;

    return strcmp(pa, pb);
}

void print_array(char *array[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("\"%s\"\n", array[i]);
    }
}

void r_realloc(char *array, int i) {
    array = realloc(array, i + sizeof(char *));
    if (array == NULL) {
        perror("realloc - fail");
        exit(EXIT_FAILURE);
    }
}

char *read_input() {
    int c;
    char *str;
    int i = 0;

    str = malloc(sizeof(char *));
    if (str == NULL) {
        perror("malloc - fail");
        exit(EXIT_FAILURE);
    }

    while ((c = getchar()) != EOF) {
        str[i] = c;
        r_realloc(str, i);
        i++;
    }
    r_realloc(str, i);
    str[i] = 0; // end of line

    return str;
}

int count_words_in(char const *string) {
    int counter = 0;
    char const *p = string;
    while (*p) {
        // space running
        while (isspace(*p) && *p) {
            p++;
        }
        if (*p == 0) break;
        p++;

        counter++;
        // word running
        while (!isspace(*p) && *p) {
            p++;
        }
        if (*p == 0) break;
        p++;
    }
    return counter;
}

void string_to_words(char const *string, char *words[]) {
    int i = 0;
    int j;
    char const *p = string;
    while (*p) {
        // space running
        while (isspace(*p) && *p) {
            p++;
        }
        if (*p == 0) break;

        words[i] = malloc(sizeof(char *)); // memory for the first letter of the word
        if (words[i] == NULL) {
            perror("malloc - fail");
            exit(EXIT_FAILURE);
        }
        j = 0;
        words[i][j] = *p; // write first letter
        j++;

        p++;
        // word running
        while (!isspace(*p) && *p) {
            r_realloc(words[i], j); // memory for a next letter
            words[i][j] = *p; // write next letter
            p++;
            j++;
        }
        words[i] = realloc(words[i], j + sizeof(char *));
        words[i][j] = 0; // end of line
        if (*p == 0) break;
        p++;
        i++;
    }
}

int main() {
    char *input_string = read_input();
    printf("\nInput string = \"%s\".\n", input_string);

    int count = count_words_in(input_string);
    printf("%d words in input string.\n", count);

    char *words[count];
    string_to_words(input_string, words);
    free(input_string); // not need input string any more
    printf("Input words:\n");
    print_array(words, count);

    size_t stringLen = sizeof(words) / sizeof(char *);
    qsort(words, stringLen, sizeof(char *), words_compare);

    printf("\nSorted words:\n");
    print_array(words, count);

    return 0;
}






