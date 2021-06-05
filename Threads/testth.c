#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int *arr, pos;
pthread_mutex_t m;

void* func(void* a){

	int* nr = (int*) a;	
	int nr1 = random() % 1000;
	int nr2 = random() % 1000;
	printf("Thread %d generated %d and %d.\n", *nr, nr1, nr2);

	if(nr1 / 100 > 0 && nr2 / 100 > 0){
		pthread_mutex_lock(&m);
		arr[pos++] = nr1;
		arr[pos++] = nr2;		
		pthread_mutex_unlock(&m);
	}


	free(nr);
	return NULL;
}

int main(int argc, char** argv){
	
	if(argc != 2){
		perror("Errors");
		exit(0);
	}
	
	srandom(time(NULL));
	int i, n = atoi(argv[1]);
	arr = malloc(sizeof(int) * (n * 2 + 1));
	pthread_t t[n + 1];

	for(i=0; i<n; i++){
		int *nr = malloc(sizeof(int));
		*nr = i;
		pthread_create(&t[i], NULL, func, (void*)nr);
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}
	printf("The array is: ");
	for(i=0; i<pos; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	free(arr);
	return 0;
}
