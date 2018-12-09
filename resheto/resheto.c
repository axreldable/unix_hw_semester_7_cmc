#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

// c99 resheto.c -pthread
// ./a.out 5 100

typedef struct node {
    int val;
    struct node *next;
} node_t;

node_t *head;
pthread_mutex_t mutex;

void print_list(node_t *head) {
    node_t *current = head;

    // skip first 2 elems
    current = current->next;
    current = current->next;

    while (current != NULL) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("\n");
}

void push(node_t *head, int val) {
    pthread_mutex_lock(&mutex);
    node_t *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
    pthread_mutex_unlock(&mutex);
}

void swap(struct node *a, struct node *b) {
    int temp = a->val;
    a->val = b->val;
    b->val = temp;
}

void bubbleSort(struct node *start) {
    int swapped, i;
    struct node *ptr1;
    struct node *lptr = NULL;

    if (start == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr) {
            if (ptr1->val > ptr1->next->val) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

char *combine(char *first, char *second) {
    char separator = ',';
    size_t newSize = strlen(first) + sizeof(separator) + strlen(second) + 1 + sizeof(separator);
    char *rez = (char *) malloc(newSize);

    strcpy(rez, first);
    strcat(rez, ",");
    strcat(rez, second);
    strcat(rez, ",");

    return rez;
}

void *sieve(void *param) {
    char *char_from = strtok(param, ",");
    char *char_to = strtok(NULL, ",");

    int from = atoi(char_from);
    int to = atoi(char_to);

//    printf("Find prime numbers from=%d, to=%d, using eratosthenes sieve.\n", from, to);

    int ar[to + 1];
    for (int i = 0; i < to + 1; i++) {
        ar[i] = i;
    }

    for (int p = 2; p < to + 1; p++) {
        if (ar[p] != 0) {
            for (int j = p * p; j < to + 1; j += p)
                ar[j] = 0;
        }
    }

    for (int i = from; i < to + 1; i++) {
        if (ar[i] != 0) {
//            printf("%d\n", ar[i]);
            push(head, ar[i]);
        }
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "USAGE: ./a.out number_of_threads=[1,5], max_number=[10, 10000] to\n");
        exit(1);
    }

    int number_of_threads = atoi(argv[1]);
    if (number_of_threads < 1 || number_of_threads > 5) {
        fprintf(stderr, "USAGE: ./a.out number_of_threads=[1,5], max_number=[10, 10000] to\n");
        exit(1);
    }

    int max_number = atoi(argv[2]);
    if (max_number < 10 || max_number > 10000) {
        fprintf(stderr, "USAGE: ./a.out number_of_threads=[1,5], max_number=[10, 10000] to\n");
        exit(1);
    }

    head = malloc(sizeof(node_t));

    pthread_t th_ids[number_of_threads];
    pthread_attr_t attrs[number_of_threads];

    int chunkSize = max_number / number_of_threads;
    for (int i = 0; i < number_of_threads; i++) {
        int start = i * chunkSize;
        int end = start + chunkSize - 1;
        if (i == number_of_threads - 1) {
            end = max_number - 1;
        }

        char from[10];
        sprintf(from, "%d", start + 1);

        char to[10];
        sprintf(to, "%d", end + 1);

        char *param = combine(from, to);

        pthread_attr_init(&attrs[i]);
        pthread_create(&th_ids[i], &attrs[i], sieve, param);
    }

    for (int i = 0; i < number_of_threads; i++) {
        pthread_join(th_ids[i], NULL);
    }

    bubbleSort(head);
    print_list(head);

    printf("\nComplete\n");
}
