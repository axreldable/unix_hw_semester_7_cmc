#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

#define SIZE 100
int print_name = 0;

// функция считывает с консоли в файл и возвращает его имя
char * createFile() {
    char * fileName = "temp.txt";
    FILE * f = fopen(fileName, "w");
    int c;
    while ((c = getchar()) != EOF) {
        fputc(c, f);
    }
    fclose(f);
    return fileName;
}

int tail(char *name, int n) {
    FILE *in;
    in = fopen(name, "r");
    if (in == NULL) {
        perror(name);
        return 0;
    }

    if (print_name) {
        printf("\n==> %s <==\n", name);
    }

    int count = 0;

    unsigned long long pos;
    char str[2 * SIZE];

    if (fseek(in, 0, SEEK_END))
        perror("fseek() failed");
    else {
        pos = ftell(in);

        while (pos) {
            if (!fseek(in, --pos, SEEK_SET)) {
                if (fgetc(in) == '\n')
                    if (count++ > n - 1)
                        break;
            } else
                perror("fseek() failed");
        }

        while (fgets(str, sizeof(str), in))
            printf("%s", str);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
// если функция без аргументов, то будем работать с временным файлом
        char * nameOfMyFile = createFile();
        tail(nameOfMyFile, 10);
        remove(nameOfMyFile); // удаляем файл
    } else {
        if (argc > 2) {
            print_name = 1;
        }

        for (int i = 1; i < argc; i++) {
            tail(argv[i], 10);
        }
    }
}
