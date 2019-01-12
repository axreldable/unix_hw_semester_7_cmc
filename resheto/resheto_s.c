#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <unistd.h>
#define THREAD_COUNT 100
unsigned long main_to_th [THREAD_COUNT][2];
struct list *th_to_main [THREAD_COUNT];
pthread_mutex_t mutex;
unsigned int prime_count;


struct node {
	unsigned long value;
	struct node *next;
};
struct list {
	struct node *begin;
	struct node *end;
	unsigned int count;
} *main_list;

void list_add (unsigned long val, struct list *ls) {
	pthread_mutex_lock (&mutex);
	struct node *temp = (struct node *) malloc (sizeof (struct node));
	pthread_mutex_unlock (&mutex);
	temp->value = val;
	temp->next = NULL;
	if (!ls->count){
		ls->begin = ls->end = temp;
	}
	else {
		ls->end->next = temp;
		ls->end = temp;
	}
	ls->count++;


}
void list_del (struct list *ls) {
	struct node *temp;
	while (ls->begin){
		temp = ls->begin;
		ls->begin = ls->begin->next;
		free (temp);
	}
	free (ls);
}

unsigned long get_begin(int id) {
	unsigned long temp;
	while (main_to_th[id][0] == 0);
	temp = main_to_th[id][0];
	main_to_th[id][0] = 0;
	return temp;
}
unsigned long get_end(int id) {
	unsigned long temp;
	while (main_to_th[id][1] == 0);
	temp = main_to_th[id][1];
	main_to_th[id][1] = 0;
	return temp;
}

void set_begin(unsigned long val, int id) {
	while (main_to_th[id][0] != 0);
	if (val == 0) { val = -1; }
	main_to_th[id][0] = val;
}
void set_end(unsigned long val, int id) {
	while (main_to_th[id][1] != 0);
	if (val == 0) { val = -1; }
	main_to_th[id][1] = val;
}



void set_return (int id, struct list *ls) {
	while (th_to_main[id] != NULL);
	th_to_main[id] = ls;
}

void get_return (int id) {
	struct list *temp;
	while (th_to_main[id] == NULL);
	temp = th_to_main[id];
	th_to_main[id] = NULL;
	if (temp->count == 0) { return; }
	main_list->end->next = temp->begin;
	main_list->end = temp->end;
	main_list->count += temp->count;
	while (temp->begin){
		printf("%lu ", temp->begin->value);
		fflush(stdout);
		temp->begin = temp->begin->next;
	}
	free (temp);
}

void * find_prime (void *arg) { 
	
	int th_id =  *(int *) arg;
	unsigned long i;
	struct node *j;
	unsigned int flag, k;
	while (1) {
		
		pthread_mutex_lock (&mutex);
		struct list *th_list = (struct list *) malloc (sizeof(struct list));
		pthread_mutex_unlock (&mutex);
		th_list->count = 0;

		unsigned long begin = get_begin (th_id);
		unsigned long end = get_end (th_id);
		if ((begin == -1) || (end == -1)) {
			set_return (th_id, th_list);
			return NULL; 
		}

		for ( i = begin; i <= end; i++) {	
			j = main_list->begin;
			k = 0;
			flag = 0;
			while ( (!flag) && (k < prime_count-1)) {
				if ((i % j->value) == 0) { flag = 1; }
				j = j->next;
				k++;
			}
			if (!flag) {
				list_add (i, th_list);
			}
		}
		set_return (th_id, th_list);
	}
}

int main (int argc, char **argv)
{
	pthread_mutex_init(&mutex, NULL);
	main_list = (struct list *) malloc (sizeof (struct list));
	if (argc < 2) {
		printf("В качеств параметров необходимо передать число нитей [макс. число > 10]\n");
		return -1;
	}
	int num_thread = atoi (argv[1]);
	unsigned long max_prime = ULONG_MAX;
	if (argc > 2) { max_prime = atoi (argv[2]); }

	unsigned long curr_max_prime = 10;

	pthread_t *th_arr;
	int thstart_result;
	th_arr = (pthread_t *) malloc (num_thread * sizeof(pthread_t));
	for (int i = 0; i < num_thread; i++) {	
		int *j = (int *) malloc (sizeof(int));
		*j = i;
		main_to_th[i][0] = main_to_th[i][1] = 0;
		th_to_main[i] = NULL;

		thstart_result = pthread_create (th_arr + i, NULL, find_prime, j);
		if (thstart_result != 0) {
			printf ("Ошибка при создании %d нити\n", i);
			return -1;
		}
		
	}

	main_list->count = 0;
	list_add (2, main_list); list_add (3, main_list); list_add (5, main_list); list_add (7, main_list);
	printf("2 3 5 7 ");

	unsigned long step;
	unsigned long temp;

	while (curr_max_prime < max_prime) {
		// подготовка входных данных для нитей
		prime_count = main_list->count;
		temp = curr_max_prime;
		if (max_prime / curr_max_prime < curr_max_prime) { curr_max_prime = max_prime; }
		else { curr_max_prime *= curr_max_prime; }

		step = (curr_max_prime - temp) / num_thread;
		for (int i = 0; i < num_thread - 1; i++) {
			set_begin (temp + 1 + i*step, i);
			set_end (temp + (i+1)*step, i);
		}
		set_begin (temp + 1 + (num_thread - 1) * step, num_thread - 1);
		set_end (curr_max_prime, num_thread - 1);

		// получение данных от нитей
		for (int i = 0; i < num_thread; i++) {
			get_return (i);
		}
	}
	list_del (main_list);
	printf("\n");

	return 0;
}

