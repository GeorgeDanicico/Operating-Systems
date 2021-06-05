#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

int main(int argc, char** argv){
	
	sem_t *sem;
	int i;
	
	struct timeval tv1, tv2;
	gettimeofday(&tv1, NULL);
	int shmid = shmget(1234, sizeof(sem_t), IPC_CREAT | 0644);
	sem = (sem_t*) shmat(shmid, NULL, 0);
	sem_init(sem, 1, 1);

	for(i=0; i<10; i++){
		int f = fork();
		if(f < 0){
			perror("Error");
		}
		else{
			if(f==0){
			sem_wait(sem);
			sleep(1);
			sem_post(sem);
			exit(0);
			}
		}
	}
	
	for(i=0; i<10; i++){
		wait(0);
	}
	sem_destroy(sem);
	
	shmdt(sem);
	shmctl(shmid, IPC_RMID, NULL);

	gettimeofday(&tv2, NULL);
    printf("The execution time is: %f.\n", (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec)/1000000);




	return 0;
}
