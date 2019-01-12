#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#define NOT_SIMPLE 1
#define RUN 1
#define SKIP 0
#define STOP -1

typedef struct {
    unsigned long long id;
    unsigned long long max_number;
    int *main_pipe;
    int *thread_pipe;
} thread_params;

typedef struct {
    int task;
    unsigned long long start;
    unsigned long long *numbers;
    unsigned long long offset;
    unsigned long long limit;
    unsigned long long *simples;
    unsigned long long simples_size;
} task;

void *thread_func(void *vargp) {
    int num = 1;
    thread_params *params = vargp;
    task t;

    while(read(params->thread_pipe[0], &t, sizeof(t)) > 0) {
        if (t.task == RUN) {
            unsigned long long number = t.start + t.offset;
            unsigned long long index = t.offset;
            for (unsigned long long i = 0; i < t.limit; i++) {
                for (unsigned long long j = 0; j < t.simples_size; j++) {
                    if (t.simples[j] * t.simples[j] > number) {
                        break;
                    }
                    if (number % t.simples[j] == 0) {
                        t.numbers[index] = NOT_SIMPLE;
                        break;
                    }
                }
                number++;
                index++;
            }
        } else if (t.task == STOP) {
            break;
        }
        write(params->main_pipe[1], &num, sizeof(int));
    }

    return NULL;
}

void check_alloc(void *ptr) {
    if (ptr == NULL) {
        perror("malloc failed");
        exit(-1);
    }
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Usage: %s threads_number [max_number]\n", argv[0]);
        return 0;
    }

    unsigned long long threads_number = atoi(argv[1]);
    unsigned long long max_number = argc == 3 ? atoi(argv[2]) : ULLONG_MAX;

    if (max_number == 0) {
        max_number = ULLONG_MAX;
    }

    if (threads_number == 0) {
        puts("Usage: threads_number > 0");
        return 0;
    }

    unsigned long long simples_size = 5;
    unsigned long long *simples = malloc(sizeof(unsigned long long) * simples_size);
    check_alloc(simples);

    simples[0] = 2;
    simples[1] = 3;
    simples[2] = 5;
    simples[3] = 7;
    simples[4] = 11;

    for (int i = 0; i < 5 && simples[i] <= max_number; i++) {
        printf("%llu\n", simples[i]);
    }

    if (max_number < 12) {
        return 0;
    }

    int main_pipe[2];
    if (pipe(main_pipe) < 0) {
        perror("Pipe error");
        return -1;
    }

    int thread_pipes[threads_number][2];
    pthread_t threads[threads_number];

    for (unsigned long long i = 0; i < threads_number; i++) {
        if (pipe(thread_pipes[i]) < 0) {
            perror("Pipe error");
            exit(-1);
        }

        thread_params *params = malloc(sizeof(thread_params));
        check_alloc(params);

        params->id = i;
        params->max_number = max_number;
        params->main_pipe = main_pipe;
        params->thread_pipe = thread_pipes[i];

        if (pthread_create(&threads[i], NULL, thread_func, params)) {
            perror("pthread_create error");
            exit(-1);
        }
    }

    unsigned long long offset = 12;
    unsigned long long limit = 11*11;
    unsigned long long actual_limit = min(limit, max_number);
    unsigned long long numbers_count = actual_limit - offset + 1;
    unsigned long long *numbers = calloc(numbers_count, sizeof(unsigned long long));
    check_alloc(numbers);

    while (offset <= max_number) {
        unsigned long long task_size = numbers_count / threads_number;
        if (task_size == 0) {
            task_size = 1;
            
            unsigned long long i = 0;
            for (; i < numbers_count; i++) {
                task t;
                t.task = RUN;
                t.start = offset;
                t.numbers = numbers;
                t.limit = task_size;
                t.offset = i;
                t.simples = simples;
                t.simples_size = simples_size;
                write(thread_pipes[i][1], &t, sizeof(t));
            }

            for (; i < threads_number; i++) {
                task t;
                t.task = SKIP;
                write(thread_pipes[i][1], &t, sizeof(t));
            }
        } else {
            unsigned long long offs = 0;
            for (unsigned long long i = 0; i < threads_number - 1; i++) {
                task t;
                t.task = RUN;
                t.start = offset;
                t.numbers = numbers;
                t.limit = task_size;
                t.offset = offs;
                t.simples = simples;
                t.simples_size = simples_size;
                write(thread_pipes[i][1], &t, sizeof(t));
                offs += task_size;
            }

            task t;
            t.task = RUN;
            t.numbers = numbers;
            t.start = offset;
            t.limit = numbers_count - offs;
            t.offset = offs;
            t.simples = simples;
            t.simples_size = simples_size;
            write(thread_pipes[threads_number - 1][1], &t, sizeof(t));
        }
        
        for (unsigned long long i = 0; i < threads_number; i++) {
            int num;
            if (read(main_pipe[0], &num, sizeof(num)) != sizeof(int)) {
                perror("read error");
                exit(-1);
            }
        }

        unsigned long long max_simple = simples[simples_size-1];
        for (unsigned long long i = 0; i < numbers_count; i++) {
            if (numbers[i] != NOT_SIMPLE) {
                simples_size++;
                simples = realloc(simples, sizeof(unsigned long long) * simples_size);
                max_simple = offset + i;
                simples[simples_size-1] = max_simple;
                printf("%llu\n", max_simple);
            }
        }
        
        offset = limit + 1;
        limit = max_simple * max_simple;
        actual_limit = min(limit, max_number);

        if (actual_limit < offset) {
            break;
        }

        numbers_count = actual_limit - offset + 1;
        numbers = realloc(numbers, sizeof(unsigned long long) * numbers_count);
        check_alloc(numbers);

        for (unsigned long long i = 0; i < numbers_count; i++) {
            numbers[i] = 0;
        }
    }

    task t;
    t.task = STOP;
    for (unsigned long long i = 0; i < threads_number; i++) {
        write(thread_pipes[i][1], &t, sizeof(t));
    }
    
    for (unsigned long long i = 0; i < threads_number; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}