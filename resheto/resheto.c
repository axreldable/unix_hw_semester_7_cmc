//#include <stdlib.h>
//#include <stdio.h>

//-pthread
//
//int main(int argc, char *argv[]) {
//    int max_number = atoi(argv[1]);
//    int thread_count = 1;
//
//    int ar[max_number + 1];
//    for (int i = 0; i < max_number + 1; i++) {
//        ar[i] = i;
//    }
//
//    for (int p = 2; p < max_number + 1; p++) {
//        if (ar[p] != 0) {
//            printf("%d\n", ar[p]);
//            for (int j = p * p; j < max_number + 1; j += p)
//                ar[j] = 0;
//        }
//    }
//
//    return 0;
//}


//#include <stdlib.h>
//#include <stdio.h>
//#include <errno.h>
//#include <pthread.h>
//#include <unistd.h>
//
//void * thread_func(void *arg)
//{
//    int i;
//    int loc_id = * (int *) arg;
//    for (i = 0; i < 4; i++) {
//        printf("Thread %i is running\n", loc_id);
//        sleep(1);
//    }
//}
//int main(int argc, char * argv[])
//{
//    int id1, id2, result;
//    pthread_t thread1, thread2;
//    id1 = 1;
//    result = pthread_create(&thread1, NULL, thread_func, &id1);
//    if (result != 0) {
//        perror("Creating the first thread");
//        return EXIT_FAILURE;
//    }
//    id2 = 2;
//    result = pthread_create(&thread2, NULL, thread_func, &id2);
//    if (result != 0) {
//        perror("Creating the second thread");
//        return EXIT_FAILURE;
//    }
//    result = pthread_join(thread1, NULL);
//    if (result != 0) {
//        perror("Joining the first thread");
//        return EXIT_FAILURE;
//    }
//    result = pthread_join(thread2, NULL);
//    if (result != 0) {
//        perror("Joining the second thread");
//        return EXIT_FAILURE;
//    }
//    printf("Done\n");
//    return EXIT_SUCCESS;
//}

//#include <stdlib.h>
//#include <stdio.h>
//#include <pthread.h>
//#include <limits.h>
//#include <unistd.h>
//#define THREAD_COUNT 100
//unsigned long main_to_th [THREAD_COUNT][2];
//struct list *th_to_main [THREAD_COUNT];
//pthread_mutex_t mutex;
//unsigned int prime_count;
//
//
//struct node {
//    unsigned long value;
//    struct node *next;
//};
//struct list {
//    struct node *begin;
//    struct node *end;
//    unsigned int count;
//} *main_list;
//
//void list_add (unsigned long val, struct list *ls) {
//    pthread_mutex_lock (&mutex);
//    struct node *temp = (struct node *) malloc (sizeof (struct node));
//    pthread_mutex_unlock (&mutex);
//    temp->value = val;
//    temp->next = NULL;
//    if (!ls->count){
//        ls->begin = ls->end = temp;
//    }
//    else {
//        ls->end->next = temp;
//        ls->end = temp;
//    }
//    ls->count++;
//
//
//}
//void list_del (struct list *ls) {
//    struct node *temp;
//    while (ls->begin){
//        temp = ls->begin;
//        ls->begin = ls->begin->next;
//        free (temp);
//    }
//    free (ls);
//}
//
//unsigned long get_begin(int id) {
//    unsigned long temp;
//    while (main_to_th[id][0] == 0);
//    temp = main_to_th[id][0];
//    main_to_th[id][0] = 0;
//    return temp;
//}
//unsigned long get_end(int id) {
//    unsigned long temp;
//    while (main_to_th[id][1] == 0);
//    temp = main_to_th[id][1];
//    main_to_th[id][1] = 0;
//    return temp;
//}
//
//void set_begin(unsigned long val, int id) {
//    while (main_to_th[id][0] != 0);
//    if (val == 0) { val = -1; }
//    main_to_th[id][0] = val;
//}
//void set_end(unsigned long val, int id) {
//    while (main_to_th[id][1] != 0);
//    if (val == 0) { val = -1; }
//    main_to_th[id][1] = val;
//}
//
//
//
//void set_return (int id, struct list *ls) {
//    while (th_to_main[id] != NULL);
//    th_to_main[id] = ls;
//}
//
//void get_return (int id) {
//    struct list *temp;
//    while (th_to_main[id] == NULL);
//    temp = th_to_main[id];
//    th_to_main[id] = NULL;
//    if (temp->count == 0) { return; }
//    main_list->end->next = temp->begin;
//    main_list->end = temp->end;
//    main_list->count += temp->count;
//    while (temp->begin){
//        printf("%lu ", temp->begin->value);
//        fflush(stdout);
//        temp->begin = temp->begin->next;
//    }
//    free (temp);
//}
//
//void * find_prime (void *arg) {
//
//    int th_id =  *(int *) arg;
//    unsigned long i;
//    struct node *j;
//    unsigned int flag, k;
//    while (1) {
//
//        pthread_mutex_lock (&mutex);
//        struct list *th_list = (struct list *) malloc (sizeof(struct list));
//        pthread_mutex_unlock (&mutex);
//        th_list->count = 0;
//
//        unsigned long begin = get_begin (th_id);
//        unsigned long end = get_end (th_id);
//        if ((begin == -1) || (end == -1)) {
//            set_return (th_id, th_list);
//            return NULL;
//        }
//
//        for ( i = begin; i <= end; i++) {
//            j = main_list->begin;
//            k = 0;
//            flag = 0;
//            while ( (!flag) && (k < prime_count-1)) {
//                if ((i % j->value) == 0) { flag = 1; }
//                j = j->next;
//                k++;
//            }
//            if (!flag) {
//                list_add (i, th_list);
//            }
//        }
//        set_return (th_id, th_list);
//    }
//}
//
//int main (int argc, char **argv)
//{
//    pthread_mutex_init(&mutex, NULL);
//    main_list = (struct list *) malloc (sizeof (struct list));
//    if (argc < 2) {
//        printf("В качеств параметров необходимо передать число нитей [макс. число > 10]\n");
//        return -1;
//    }
//    int num_thread = atoi (argv[1]);
//    unsigned long max_prime = ULONG_MAX;
//    if (argc > 2) { max_prime = atoi (argv[2]); }
//
//    unsigned long curr_max_prime = 10;
//
//    pthread_t *th_arr;
//    int thstart_result;
//    th_arr = (pthread_t *) malloc (num_thread * sizeof(pthread_t));
//    for (int i = 0; i < num_thread; i++) {
//        int *j = (int *) malloc (sizeof(int));
//        *j = i;
//        main_to_th[i][0] = main_to_th[i][1] = 0;
//        th_to_main[i] = NULL;
//
//        thstart_result = pthread_create (th_arr + i, NULL, find_prime, j);
//        if (thstart_result != 0) {
//            printf ("Ошибка при создании %d нити\n", i);
//            return -1;
//        }
//
//    }
//
//    main_list->count = 0;
//    list_add (2, main_list); list_add (3, main_list); list_add (5, main_list); list_add (7, main_list);
//    printf("2 3 5 7 ");
//
//    unsigned long step;
//    unsigned long temp;
//
//    while (curr_max_prime < max_prime) {
//        // подготовка входных данных для нитей
//        prime_count = main_list->count;
//        temp = curr_max_prime;
//        if (max_prime / curr_max_prime < curr_max_prime) { curr_max_prime = max_prime; }
//        else { curr_max_prime *= curr_max_prime; }
//
//        step = (curr_max_prime - temp) / num_thread;
//        for (int i = 0; i < num_thread - 1; i++) {
//            set_begin (temp + 1 + i*step, i);
//            set_end (temp + (i+1)*step, i);
//        }
//        set_begin (temp + 1 + (num_thread - 1) * step, num_thread - 1);
//        set_end (curr_max_prime, num_thread - 1);
//
//        // получение данных от нитей
//        for (int i = 0; i < num_thread; i++) {
//            get_return (i);
//        }
//    }
//    list_del (main_list);
//    printf("\n");
//
//    return 0;
//}

/* System programming:
 Find the prime numbers between min_search and max_search
 Used algorithm: improved "Sieve of Eratosthenes" algorithm
 gcc -lpthread -lm -Wall prime.c -o prime  */
//#include <pthread.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//void *runner(void *param); /* the thread */
//
//int main(int argc, char *argv[]) {
//
//    //Verify two args were passed in
//    if(argc < 2) {
//        fprintf(stderr, "USAGE: ./prime.out <Integer value>\n");
//        exit(1);
//    }
//
//    //verify the input is greater then or equal to two
//    if(atoi(argv[1]) < 2) {
//        fprintf(stderr, "USAGE: %d must be >= 2\n", atoi(argv[1]));
//        exit(1);
//    }
//
//    pthread_t tid;       //Thread ID
//    pthread_attr_t attr; //Set of thread attributes
//
//    printf("Prime Numbers: ");
//
//    //Get the default attributes
//    pthread_attr_init(&attr);
//    //Create the thread
//    pthread_create(&tid,&attr,runner,argv[1]);
//    //Wait for the thread to exit
//    pthread_join(tid,NULL);
//    printf("\nComplete\n");
//}
//
////The thread will begin control in this function
//void *runner(void *param) {
//    int i,j,upper = atoi(param);
//    /* Check to see if a number is prime */
//    for(i = 2; i < upper; i++) {
//        int trap = 0;
//        /* Check each number for divisibility */
//        for(j = 2; j < i; j++) {
//            int result = i % j;
//            /* If any of the numbers divide cleanly
//                then this number is not prime. So
//                stop checking. */
//            if(result == 0) {
//                trap = 1;
//                break;
//            }
//        }
//        //No numbers divided cleanly so this number must be prime
//        if(trap == 0) {
//            printf("[%d] ", i);
//        }
//    }
//    //Exit the thread
//    pthread_exit(0);
//}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

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
    char* char_from = strtok(param, ",");
    char* char_to = strtok(NULL, ",");

    int from = atoi(char_from);
    int to = atoi(char_to);

    printf("Find prime numbers from=%d, to=%d, using eratosthenes sieve.\n", from, to);

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
            printf("%d\n", ar[i]);
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

    pthread_t th_ids[number_of_threads];
    pthread_attr_t attrs[number_of_threads];

    int chunkSize = max_number / number_of_threads;
    for (int i = 0; i < number_of_threads; i++) {
        int start = i * chunkSize;
        int end = start + chunkSize - 1;
        if (i == number_of_threads - 1) {
            end = max_number - 1;
        }

//        printf("%d - %d", start+1, end+1);

        char from[10];
        sprintf(from, "%d", start+1);

        char to[10];
        sprintf(to, "%d", end+1);

        char *param = combine(from, to);
        //Get the default attributes
        pthread_attr_init(&attrs[i]);
        //Create the thread
        pthread_create(&th_ids[i], &attrs[i], sieve, param);
        printf("\n");
    }

    for (int i = 0; i < number_of_threads; i++) {
        pthread_join(th_ids[i], NULL);
    }

    printf("\nComplete\n");
}
