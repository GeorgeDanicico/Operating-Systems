#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_barrier_t b;
pthread_mutex_t m;

typedef struct{
	int index;
	int *sum;
}data;

void *func(void *a){

	data *d = ((data *)a);
	int addnr = random()%10 + 1;
	pthread_mutex_lock(&m);
	*d->sum += addnr;
	pthread_mutex_unlock(&m);

	pthread_barrier_wait(&b);
	int subnr = random() % 	15 + 1;
	printf("Thread %d generated: %d and %d ", d->index, addnr, subnr);
	if(subnr <= *d->sum){
		pthread_mutex_lock(&m);
		*d->sum -=subnr;
		printf("and substracted from the sum.\n");
		pthread_mutex_unlock(&m);
	}
	else{
		printf("and couldn't substract from the sum.\n");
	}	 

	return NULL;
}

int main(int argc, char **argv){
	
	int i, n = atoi(argv[1]);
	srandom(time(NULL));
	data* arg = malloc(sizeof(data) * n);
	int* s = malloc(sizeof(int));
	*s = 0;
	for(i=0; i<n; i++){
		arg[i].index = i;
		arg[i].sum = s;
	}
	
	pthread_barrier_init(&b, NULL, n);
	pthread_mutex_init(&m, NULL);
	pthread_t T[n+1];
	for(i=0; i<n; i++){
		pthread_create(&T[i], NULL, func, (void*)(arg+i));
	}

	 for(i=0; i<n; i++){
         pthread_join(T[i], NULL);
     }

	pthread_mutex_destroy(&m);
	pthread_barrier_destroy(&b);
	
	printf("The sum is: %d.\n", *s);
	free(s);
	free(arg);
	return 0;
}
