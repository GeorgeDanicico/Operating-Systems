#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct{
	sem_t* sem;
	int* index, n;
	pthread_barrier_t *barrier;

}data;

void* func(void *a){

	data d = *((data*)a);
	int i;
	printf("Thread %d waiting...\n", *(d.index));
	pthread_barrier_wait(d.barrier);
	for(i=0; i<d.n; i++){
		sem_wait(&d.sem[i]);
		printf("Thread %d entered checkpoint: %d.\n", *(d.index),i);

		sem_post(&d.sem[i]);
	}
	printf("Thread %d finished...\n", *(d.index));

	return NULL;
}

int main(int argc, char** argv){
	if(argc < 1){
		perror("error");
		exit(0);
	}

	int n = atoi(argv[1]);
	int m = 1, i;
	for(i=0; i<n; i++){
		m*=2;
	}
	int value = m/2;
	pthread_barrier_t *b = malloc(sizeof(pthread_barrier_t));	
	pthread_barrier_init(b, NULL, m);
	data* d = malloc(sizeof(data)*m);
	sem_t* sems = malloc(sizeof(sem_t)*n);

	for(i=0; i<n;i++){
		sem_init(&sems[i], 0, value);
		value/=2;
	}

	for(i=0; i<m;i++){
		d[i].n = n;
		d[i].index = malloc(sizeof(int));
		*(d[i].index) = i;
		d[i].barrier = b;
		d[i].sem = sems;
	}

	pthread_t t[m + 1];

	for(i=0; i<m; i++){
		pthread_create(&t[i], NULL, func, (void*)(d+i));
	}

	for(i=0; i<m; i++){
		pthread_join(t[i], NULL);
	}

	for(i=0; i<n;i++){
		sem_destroy(&sems[i]);
	}	

	for(i=0; i<m;i++){
        free(d[i].index);
    }
	free(sems);
	free(d);
	pthread_barrier_destroy(b);
	free(b);
		
	return 0;
}
