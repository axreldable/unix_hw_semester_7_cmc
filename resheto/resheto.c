#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

// c99 resheto.c -pthread

// time ./a.out 1 1000000
// real    0m32,684s
// user    0m32,679s
// sys     0m0,005s

// time ./a.out 5 1000000
// real    0m22,689s
// user    0m48,603s
// sys     0m0,238s

// time ./a.out 8 1000000
// real    0m9,345s
// user    0m28,986s
// sys     0m0,237s

// time ./a.out 20 1000000
// real    0m11,093s
// user    0m47,468s
// sys     0m0,313s

#define TRUE 1
#define FALSE 0

int number_of_threads;
long long int max_number;

typedef struct node {
    long long int val;
    struct node *next;
} node_t;
node_t *simple_numbers;


pthread_mutex_t mutex;

struct arg_struct {
    long long int start;
    long long int end;
};

void print_list(node_t *head) {
    node_t *current = head;

    current = current->next;
    while (current != NULL) {
        printf("%llu ", current->val);
        current = current->next;
    }
    printf("\n");
}

void push(node_t *head, long long int val) {
    pthread_mutex_lock(&mutex);

    node_t *current = head;
    while (current->next != NULL && current->next->val < val) {
        current = current->next;
    }

    if (current->next == NULL) {
        current->next = malloc(sizeof(node_t));
        current->next->val = val;
        current->next->next = NULL;
    } else {
        node_t *tmp = malloc(sizeof(node_t));
        tmp->val = val;
        tmp->next = current->next;
        current->next = tmp;
    }

    pthread_mutex_unlock(&mutex);
}

void add_to_common(node_t *from, node_t *to) {
    node_t *current = from;

    current = current->next;
    while (current != NULL) {
        push(to, current->val);
        current = current->next;
    }
}

void parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: ./a.out number_of_threads(from 1), max_number(from 10)\n");
        exit(1);
    }

    number_of_threads = atoi(argv[1]);
    if (number_of_threads < 1) {
        number_of_threads = 1;
    }

    if (argc == 2) {
//        max_number = LLONG_MAX;
        max_number = 10000000000000000;
    } else {
        max_number = atoll(argv[2]);
        if (max_number <= 10) {
            printf("Use max_number=%d.\n", 10);
            max_number = 10;
        }
    }
}

void init_simple_numbers() {
    simple_numbers = malloc(sizeof(node_t));
    push(simple_numbers, 2);
    push(simple_numbers, 3);
    push(simple_numbers, 5);
    push(simple_numbers, 7);
}

void *thread_func(void *params) {
    struct arg_struct *args = (struct arg_struct *) params;

    long long int from = args->start;
    long long int to = args->end;

//    printf("Find prime numbers from=%llu, to=%llu.\n", from, to);

    node_t * local_simple_numbers = malloc(sizeof(node_t));
    for(long long int i = from; i <= to; i++) {
        node_t *current = simple_numbers;

        current = current->next;
        int is_prime = TRUE;
        while (current != NULL) {
            if (i % current->val == 0) {
                is_prime = FALSE;
            }
            current = current->next;
        }
        if (is_prime) {
            push(local_simple_numbers, i);
        }
    }

    add_to_common(local_simple_numbers, simple_numbers);

    pthread_exit(0);
}

int add_new_simple_numbers_using_several_threads(long long int begin_number, long long int end_number) {
    pthread_t th_ids[number_of_threads];
    pthread_attr_t attrs[number_of_threads];
    struct arg_struct params[number_of_threads];

    long long int chunkSize = (end_number - begin_number) / number_of_threads;
    for (int i = 0; i < number_of_threads; i++) {
        long long int start = i * chunkSize + begin_number + 1;
        long long int end = start + chunkSize - 1;

        params[i].start = start;
        params[i].end = end;

        pthread_attr_init(&attrs[i]);
        pthread_create(&th_ids[i], &attrs[i], thread_func, (void *) &params[i]);
    }

    for (int i = 0; i < number_of_threads; i++) {
        pthread_join(th_ids[i], NULL);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    init_simple_numbers();

    long long int begin_number = 10;
    long long int square = begin_number * begin_number;
    long long int end_number = (square > max_number) ? max_number : square;

    int need_process = TRUE;
    while (need_process) {
        if (add_new_simple_numbers_using_several_threads(begin_number, end_number) != 0) {
            printf("Fail during adding new simple numbers: from=%llu, to=%llu\n", begin_number, end_number);
            exit(1);
        }

        if (end_number == max_number) {
            need_process = FALSE;
        }
        begin_number = end_number;
        square = begin_number * begin_number;
        end_number = (square > max_number) ? max_number : square;
    }

    print_list(simple_numbers);

    printf("\nComplete\n");
}
