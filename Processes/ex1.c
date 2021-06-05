#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char** argv){
	struct timeval tv1, tv2;
	if(argc < 2){
		perror("Invalid input");
	}
	gettimeofday(&tv1, NULL);
	if(fork() == 0){
		execvp(argv[1], argv + 1);
		exit(1);
	}
	wait(0);
	gettimeofday(&tv2, NULL);
	
	printf("The execution time is: %f seconds.\n", (double)(tv2.tv_usec - tv1.tv_usec)/1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	



	return 0;
}
