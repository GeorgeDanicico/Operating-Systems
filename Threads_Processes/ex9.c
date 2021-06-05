#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_barrier_t b;
int* array;
typedef struct{
	int idx;
	int size;
	pthread_mutex_t *m;
}data;

void *func(void *a){
	data d = *((data*) a);
	int nr = random() % 100 + 1;
	printf("Thread %d generated %d.\n", d.idx, nr);
	array[d.idx]=nr;
	pthread_barrier_wait(&b);

	int maxim=0;
	pthread_mutex_lock(d.m);
	int i, stop = 1;
	while(stop == 1){
		maxim = 0;
		for(i=0; i<d.size; i++){
			if(maxim < array[i])
				maxim = array[i];
		}
		if(maxim > array[d.idx]){
			for(i=0; i<d.size; i++){
				if(array[i] == maxim){
					array[i] = abs(maxim-array[d.idx]);
					printf("Thread %d replaced.\n", d.idx);
				}
			
			}
		}
		if(maxim == array[d.idx]) stop = -1;

	}
	pthread_mutex_unlock(d.m);

	return NULL;
}

int main(int argc, char** argv){

	if(argc != 2){
		perror("Error");
		exit(0);
	}

	srandom(getpid());
	int n = atoi(argv[1]);
	int i=0;
	pthread_t t[n+1];
	data *arg = malloc(sizeof(data)*n);
	pthread_mutex_t *mutexes = malloc(sizeof(pthread_mutex_t));
	
	pthread_barrier_init(&b, NULL, n);
	
	pthread_mutex_init(mutexes, NULL);
	array = malloc(sizeof(int)*n);


	for(i=0; i<n; i++){
		arg[i].idx = i;arg[i].size=n;
		arg[i].m = mutexes;
		pthread_create(&t[i], NULL, func, (void*)(arg+i));
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}
	pthread_mutex_destroy(mutexes);
	pthread_barrier_destroy(&b);

	free(mutexes);
	free(arg);	
	free(array);
	return 0;
}
