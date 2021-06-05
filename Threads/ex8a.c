#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int total_sum, n, m;
pthread_mutex_t M;
void* func(void* a){
	int i,*row = (int*) a;
	
	pthread_mutex_lock(&M);
	for(i=0;i<m;i++){
		total_sum += row[i];
	}

	pthread_mutex_unlock(&M);
	return NULL;
}

int main(int argc, char** argv){

	int i,j, **matrix;
	FILE* f;
	f = fopen("matrix.txt", "r");
	fscanf(f, "%d", &n);
	fscanf(f, "%d", &m);
	pthread_t t[n+1];
	pthread_mutex_init(&M, NULL);

	matrix = (int**)malloc(sizeof(int*)*n);
	for(i=0; i<n;i++){
		matrix[i] = malloc(sizeof(int)*m);
	}

	// matrix reading
	for(i=0;i<n;i++){
		for(j=0; j<m;j++){
			fscanf(f, "%d", &matrix[i][j]);
		}
	}

	for(i=0;i<n;i++){
		pthread_create(&t[i], NULL, func, (void*)matrix[i]);
	}

	for(i=0;i<n;i++){
        pthread_join(t[i], NULL); 
    }
	printf("The total sum is: %d \n", total_sum);
	pthread_mutex_destroy(&M);
 	
	for(i=0; i<n;i++){
       	free(matrix[i]);
    }
	free(matrix);
	fclose(f);
	
	return 0;
}
