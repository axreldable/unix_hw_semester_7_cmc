#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

int wcForFile (char *);
char * createFile();

// глобальные переменные
int numberOfLines = 0;
int numberOfWords = 0;
int numberOfSymbols = 0;

int totalnumberOfLines = 0;
int totalnumberOfWords = 0;
int totalnumberOfSymbols = 0;

int main(int args, char ** argv) {
    if (args == 1) {
// если функция без аргументов, то будем работать с временным файлом
        char * nameOfMyFile = createFile();
        wcForFile(nameOfMyFile);
        remove(nameOfMyFile); // удаляем файл
        printf(" %d %d %d\n", numberOfLines, numberOfWords, numberOfSymbols);
    }
    else {
        int i;
        for (i = 1; i < args; i++) {
            if (wcForFile(argv[i])) // выводим только если файл существовал
                printf(" %d %d %d %s\n", numberOfLines, numberOfWords, numberOfSymbols, argv[i]);
            if (args > 2) {
                totalnumberOfLines += numberOfLines;
                totalnumberOfWords += numberOfWords;
                totalnumberOfSymbols += numberOfSymbols;
                numberOfLines = 0;
                numberOfWords = 0;
                numberOfSymbols = 0;
            }
        }
        if (args > 2) {
            printf(" %d %d %d %s\n", totalnumberOfLines, totalnumberOfWords, totalnumberOfSymbols, "total");
        }
    }
}

// присваивает глобальным переменным нужную информацию
int wcForFile(char * name) {
    FILE *f; // ссылка на файл
    f = fopen(name, "r"); // открываем файл для чтения
    // если файла не было ошибка
    if (f == NULL) {
        perror(name);
        return 0;
    }

    // количество строк совпадает с количеством символов '\n'
    // все символы до EOF - символы
    // для подстчета количества слов, разделенных пробелами
    // сраниваем текущий и предыдущий символы:
    // если предыдущий - разделитель, а текущий - нет, значит
    // началось новое слово
    int prev = fgetc(f);
    if (prev == EOF) return 0;
    //putchar(prev);
    numberOfSymbols += 1;
    if (prev == L'\n') numberOfLines += 1;
    if (prev != L'\n' || !iswspace(prev)) numberOfWords += 1; // начало первого слова
    int tek;
    while((tek = fgetc(f)) != EOF) {
        if (((iswspace(prev)) || (prev == L'\n')) && ((!iswspace(tek)) && (tek != L'\n'))) {
            numberOfWords += 1;
        }
        numberOfSymbols += 1;
        if (tek == L'\n') numberOfLines += 1;
        prev = tek;
    }
    return 1;
}

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
