#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


pthread_mutex_t m;
typedef struct{
	int idx;
	int fifo;
}data;

typedef struct{
	int idx;
	int nr1;
	int nr2;

}response;

void *func(void *a){
	data *d = (data *)a;
	
	int nr1 = random() % 100 + 1;
	int nr2 = random() % 100 + 1;
	response r;
	r.idx = d->idx;
	r.nr1=nr1;
	r.nr2=nr2;
	pthread_mutex_lock(&m);
	write(d->fifo, &r, sizeof(response));
	pthread_mutex_unlock(&m);
		
	return NULL;
}

int main(int argc, char** argv){
	
	char* fifoname="./abc";
	mkfifo(fifoname, 0600);
	srandom(time(NULL));
	int n = atoi(argv[1]);

	if(fork()==0){
		int i, f1 = open(fifoname, O_RDONLY);
		response r;
		for(i=0; i<n; i++){
		read(f1,&r, sizeof(response));

		double average = (double)(r.nr1 + r.nr2)/2;
		printf("Thread %d generate %d and %d and the average is %f.\n", r.idx, r.nr1, r.nr2, average);
		}

		close(f1);
		exit(0);
	}
	int i;
	pthread_t t[n+1];

	data* arg = malloc(sizeof(data)*n);
	int f2 = open(fifoname, O_WRONLY);
	for(i=0; i<n; i++){
		arg[i].idx = i;
		arg[i].fifo = f2;
		pthread_create(&t[i], NULL, func, (void*)(arg+i));
	}

	for(i=0; i<n; i++){
		pthread_join(t[i], NULL);
	}
	pthread_mutex_destroy(&m);	
	wait(0);
	close(f2);
	free(arg);
	unlink(fifoname);
	
	return 0;
}
