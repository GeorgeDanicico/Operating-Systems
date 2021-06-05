#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int *freq;
pthread_mutex_t m;
void* func(void *a){
	int nr = *((int* )a);
	pthread_mutex_lock(&m);
	freq[nr]++;
	//printf("%d", nr);
	free(a);
	pthread_mutex_unlock(&m);
	return NULL;
}

int main(int argc, char** argv){

	int i;
	freq = malloc(sizeof(int) * 11);
	memset(freq, 0, 11);
	pthread_t t[argc+1];

	pthread_mutex_init(&m, NULL);

	for(i=1; i<argc; i++){
		int *nr = malloc(sizeof(int));
	    *(nr) = atoi(argv[i]);
		pthread_create(&t[i], NULL, func, (void*)nr);
	}

	for(i=1; i<argc; i++){
		pthread_join(t[i], NULL);
	}

    for(i=0;i<10;i++){
        printf("Frequency for %d: %d \n", i, freq[i]);
    }

	pthread_mutex_destroy(&m);
	free(freq);
	return 0;
}

