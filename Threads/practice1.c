#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n;
int* array;
pthread_mutex_t* m;

void* func(void *a){
	
	int x = random() % (n - 1) + 1;
	int y = random() % (n - 1) + 1;
	while(x == y){
		y = random() % (n - 1) + 1;
	}
	int aux;
	if(x > y){
		aux = x;
		x = y;
		y = aux;
	}
	pthread_mutex_lock(&m[x]);
	pthread_mutex_lock(&m[y]);

	int s = array[x] + array[y];
	int p = array[x] * array[y];
	
	printf("Thread generated x: %d and y: %d.\n", x, y);
	array[x] = s;
	array[y] = p;
	int i;
	printf("The new array is: ");
	for(i=1; i<n; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
	
    pthread_mutex_unlock(&m[x]);
    pthread_mutex_unlock(&m[y]);


	return NULL;
}

int main(int argc, char **argv){

	pthread_t t[argc + 1];
	srandom(time(NULL));
	int i;
	n = argc;
	
	array = malloc(sizeof(int)*n);
	m = malloc(sizeof(pthread_mutex_t) * n);

	for(i=1; i < n;i++){
		array[i] = atoi(argv[i]);
		pthread_mutex_init(&m[i], NULL);
	}
	
	for(i=1; i<n; i++){
		pthread_create(&t[i], NULL, func, NULL);
	}

	for(i=1; i<n; i++){
        pthread_join(t[i], NULL);                         
    }

	for(i=1; i<n; i++){
		pthread_mutex_destroy(&m[i]);
	}

	free(m);
	free(array);

	return 0;
}
