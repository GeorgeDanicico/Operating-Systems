#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *arr, n, pos, turn;
pthread_mutex_t m;
pthread_cond_t c;

void* func1(void* a){
	pthread_mutex_lock(&m);
	if(turn != 1) pthread_cond_wait(&c, &m);

	while(pos < n){
		turn = 1 - turn;
		int i, nr = (random() % 100)/2 * 2 + 1;
		arr[pos] = nr;
		pos++;

		printf("Thread 1: ");
		for(i=0; i<pos; i++){
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

void* func2(void* a){
	pthread_mutex_lock(&m);
	if(turn != 0) pthread_cond_wait(&c, &m);

	while(pos < n){
          turn = 1 - turn;
          int i, nr = (random() % 100)/2 * 2;
          arr[pos] = nr;
          pos++;
 
          printf("Thread 2: ");
          for(i=0; i<pos; i++){
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
	
	printf("Enter a number: ");
	scanf("%d", &n);
	arr = malloc(sizeof(int) * n);
	pos = 0;

	pthread_t t[3];
	srandom(time(NULL));
	turn = random() % 2;
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);

	pthread_create(&t[1], NULL, func1, NULL);
	pthread_create(&t[2], NULL, func2, NULL);

	

	pthread_join(t[1], NULL);
	pthread_join(t[2], NULL);

	pthread_cond_destroy(&c);
	pthread_mutex_destroy(&m);
	free(arr);
	return 0;
}
