#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int *arr,pos = 0, n;
pthread_mutex_t m;
pthread_cond_t c;

void *f1(void *a){
	int nr = (random() % 100)/2 * 2;
	
	pthread_mutex_lock(&m);
	if(pos % 2 == 0) pthread_cond_wait(&c, &m);
	while(pos < n){
		
		arr[pos] = nr;
		pos++;
		int i; nr = (random() % 100)/2 * 2;
		
		printf("T1: ");
		for(i=0; i < pos; i++){
			printf("%d ", arr[i]);
		}
		printf("\n");
		pthread_cond_signal(&c);
        pthread_cond_wait(&c, &m);
	}
	pthread_mutex_unlock(&m);
	pthread_cond_signal(&c);
	return NULL;
}

void *f2(void *a){
	int nr = (random() % 100)/2 * 2 + 1;
	pthread_mutex_lock(&m);
    if(pos % 2 == 1) pthread_cond_wait(&c, &m);
	while(pos < n){
        arr[pos] = nr;
        pos++;
        nr = (random() % 100)/2 * 2 + 1;
		int i;
		printf("T2: ");
        for(i=0; i < pos; i++){
            printf("%d ", arr[i]);
        }
        printf("\n");

		pthread_cond_signal(&c);
		pthread_cond_wait(&c, &m);
   	}
    pthread_mutex_unlock(&m);
	pthread_cond_signal(&c);

	return NULL;
}


int main(int argc, char** argv){
	srandom(time(NULL));	
	pthread_t t1, t2;
	
	printf("Enter the number: ");
	scanf("%d", &n);

	pthread_cond_init(&c, NULL);
	pthread_mutex_init(&m, NULL);

	arr = malloc(sizeof(int) * n ) ;

	pthread_create(&t1, NULL, f1, (void*)arr);
	pthread_create(&t2, NULL, f2, (void*)arr);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	free(arr);
	pthread_cond_destroy(&c);
	pthread_mutex_destroy(&m);


}
