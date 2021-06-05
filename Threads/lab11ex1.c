#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
	int *thread_index;
	int *race_spots;
	pthread_mutex_t *m;
	pthread_barrier_t *barrier;
}race;

void *func(void *a){
	
	race r = *((race *) a);

	int m = *(r.race_spots), i;
	printf("Thread %d waiting. \n", *(r.thread_index));
	pthread_barrier_wait(r.barrier);
	for(i=1; i<=m; i++){
		pthread_mutex_lock(&r.m[i-1]);
		printf("Thread %d entered checkpoint %d. \n", *(r.thread_index), i);
		usleep(10000);
		pthread_mutex_unlock(&r.m[i-1]);
	}
	printf("Thread %d finished. \n", *(r.thread_index));

	return NULL;
}

int main(int argc, char** argv){

	if(argc < 2){
		perror("Invalid command line arg.");
		exit(1);
	}

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int i;	

	race *r = malloc(sizeof(race) * n);

	pthread_mutex_t *mutexes = malloc(sizeof(pthread_mutex_t)*m);
	for(i=0; i<m; i++){
		pthread_mutex_init(&mutexes[i], NULL);
	}
	
	pthread_barrier_t *b = malloc(sizeof(pthread_barrier_t));
	pthread_barrier_init(b, NULL, n);

	for(i=0; i<n; i++){
		r[i].barrier = b;
		r[i].m = mutexes;
		r[i].thread_index = malloc(sizeof(int));
     	r[i].race_spots = malloc(sizeof(int));
      	*(r[i].race_spots) = m;
		*(r[i].thread_index) = i+1;
	}
	pthread_t t[n+1];

	for(i=0; i<n; i++){
		pthread_create(&t[i], NULL, func, (void*)(r+i));
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}
	for(i = 0; i<m; i++){
		pthread_mutex_destroy(&mutexes[i]);
	}
	pthread_barrier_destroy(b);
	free(b);
	free(mutexes);

	for(i=0; i<n; i++){
        free(r[i].thread_index);
        free(r[i].race_spots);
    }	
	free(r);

	return 0;
}
