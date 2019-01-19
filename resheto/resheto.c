#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

// c99 resheto.c -pthread

// time ./a.out 1 1000000
// real    0m11,483s
// user    0m11,209s
// sys     0m0,008s

// time ./a.out 8 1000000
// real    0m2,663s
// user    0m18,219s
// sys     0m0,024s

#define TRUE 1
#define FALSE 0

int number_of_threads;
long long int max_number;

typedef struct node {
    long long int val;
    struct node *next;
} node_t;

typedef struct list {
    struct node *begin;
    struct node *end;
    long long int count;
} list_t;

list_t *simple_numbers;

struct arg_struct {
    long long int start;
    long long int end;
    list_t *local_prime_numbers;
};

void print_list(list_t *list) {
    node_t *current = list->begin;

    while (current != NULL) {
        printf("%llu ", current->val);
        current = current->next;
    }
    printf("\n");
}

void push(list_t *list, long long int val) {
    node_t *temp = malloc(sizeof(node_t));
    temp->val = val;
    temp->next = NULL;
    if (!list->count){
        list->begin = list->end = temp;
    } else {
        list->end->next = temp;
        list->end = temp;
    }
    list->count++;
}

void add_to_common(list_t *from, list_t *to) {
    node_t *current = from->begin;

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
    simple_numbers = malloc(sizeof(list_t));
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

    for(long long int i = from; i <= to; i++) {
        node_t *current = simple_numbers->begin;

        int is_prime = TRUE;
        while (current != NULL) {
            if (i % current->val == 0) {
                is_prime = FALSE;
            }
            current = current->next;
        }
        if (is_prime) {
            push(args->local_prime_numbers, i);
        }
    }

    pthread_exit(0);
}

int add_new_simple_numbers_using_several_threads(long long int begin_number, long long int end_number) {
    pthread_t th_ids[number_of_threads];
    pthread_attr_t attrs[number_of_threads];
    struct arg_struct params[number_of_threads];

    list_t *local_pr_ar[number_of_threads];
    for (int j = 0; j < number_of_threads; ++j) {
        local_pr_ar[j] = malloc(sizeof(list_t));
    }

    long long int chunkSize = (end_number - begin_number) / number_of_threads;
    for (int i = 0; i < number_of_threads; i++) {
        long long int start = i * chunkSize + begin_number + 1;
        long long int end = start + chunkSize - 1;

        params[i].start = start;
        params[i].end = end;
        params[i].local_prime_numbers = local_pr_ar[i];

        pthread_attr_init(&attrs[i]);
        pthread_create(&th_ids[i], &attrs[i], thread_func, (void *) &params[i]);
    }

    for (int i = 0; i < number_of_threads; i++) {
        pthread_join(th_ids[i], NULL);
    }

    for (int k = 0; k < number_of_threads; ++k) {
//        print_list(local_pr_ar[k]);
        add_to_common(local_pr_ar[k], simple_numbers);
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
