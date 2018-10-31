#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char ** argv) {

    if (argc < 3) {
        puts("Usage:...");
        return 0;
    }

    int p[2];
    pipe(p);

    if (fork() == 0) {
        // son 1
        puts("son 1");
        dup2(p[1], 1);
        close(p[1]);
        close(p[0]);
        execlp(argv[1], argv[1], NULL);
    } else {
        close(p[1]);
        if (fork() == 0) {
            // son 2
            puts("son 2");
            dup2(p[0], 0);
            close(p[0]);
            execlp(argv[2], argv[2], NULL);
        } else {
            // parent
            puts("parent");
            close(p[0]);
            wait(NULL);
            wait(NULL);
            puts("All is done");
        }
    }
    return 0;
}

int fain() {
    // надо вызывать exec в сыне
    execlp("ls", "ls", NULL); // второй ls - это argv[0], NULL - это argv[1]
    fprintf(stderr, "Done\n"); // это не сработало - потому что exec уничтожил нашу программу
    exit(0);
}

int lain(int argc, char ** argv) {
if (argc < 2) {
        puts("Usage:...");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (fork() == 0) {
            execlp(argv[i], argv[i], NULL);
            exit(1);
        } else {
            wait(NULL); // добавили синхронизацию, чтобы Done выводилось после выполнения команды
            fprintf(stderr, "Done\n");
        }
    }
    exit(0);
}
