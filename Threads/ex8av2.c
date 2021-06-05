#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int n, m, total_sum;
pthread_mutex_t M;

typedef struct {
	int row, *column;
}matrix;

void *func( void* a){

	matrix row = *((matrix*) a);
	int local_sum = 0;
	int i;
	for(i=0; i<m; i++){
		local_sum += row.column[i];
	}

	pthread_mutex_lock(&M);
	printf("On row %d the sum is %d.\n", row.row, local_sum);
	total_sum+=local_sum;
	pthread_mutex_unlock(&M);

	return NULL;
}

int main(){
		
	int i, j;
	FILE* f = fopen("matrix.txt", "r");
	fscanf(f, "%d", &n);
	fscanf(f, "%d", &m);
	
	matrix* ma = malloc(sizeof(matrix) * n);
	for(i=0; i<n; i++){
		ma[i].column = malloc(sizeof(int) * m);
	}

	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			fscanf(f, "%d", &ma[i].column[j]);
		}
	}

	pthread_t t[n+1];
	
	pthread_mutex_init(&M, NULL);
	for(i=0; i<n; i++){
		ma[i].row = i;
		pthread_create(&t[i], NULL, func, (void*)(ma + i));
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}

	printf("The total sum is: %d .\n", total_sum);
	pthread_mutex_destroy(&M);
	for(i=0; i<n; i++){
		free(ma[i].column);
	}
	free(ma);

	return 0;
}	
