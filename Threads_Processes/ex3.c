#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int global_nr[1000], gsize1;
char global[1000][1000], gsize2;
sem_t sem;
pthread_mutex_t mutex;

typedef struct{
	int nr;
	char *string;
}data;	

void *func(void *a){

	data *d = ((data*)a);
	sem_wait(&sem);
	pthread_mutex_lock(&mutex);
	global_nr[gsize1++] = d->nr;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&sem);

	return NULL;
}

int main(int argc, char **argv){
	
	int i, n = atoi(argv[1]);
	pthread_t *t = malloc(sizeof(pthread_t)*n);
	data* arg = malloc(sizeof(data)*n);
	
	sem_init(&sem, 0, 4);
	pthread_mutex_init(&mutex, NULL);

	for(i=0; i<n; i++){
		int m;
		char* str = malloc(sizeof(char)*100);
		scanf("%d", &m);
		scanf("%s", str);
		arg[i].nr=m;
		arg[i].string=str;
		pthread_create(&t[i], NULL, func, (void*)(arg+i));
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&sem);
	for(i=0; i<n; i++){
		printf("%d ", global_nr[i]);
	}
	printf("\n");

	free(t);
	for(i=0; i<n; i++){
		free(arg[i].string);
	}
	free(arg);

	return 0;
}
