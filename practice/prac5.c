#include <pthread.h>
#include <stdio.h>

int a = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *f(void *p) {
    int i;

    int local = 0;
//    pthread_mutex_lock(&m);
    for (i = 0; i < 100000; i++) {
        local++;
    }
//    pthread_mutex_unlock(&m);

    pthread_mutex_lock(&m);
    a += local;
    pthread_mutex_unlock(&m);

    return p;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, f, NULL);
    pthread_create(&t2, NULL, f, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("a=%d\n", a);
}
