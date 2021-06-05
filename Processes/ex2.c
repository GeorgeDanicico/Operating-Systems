#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv){

	int p2c[2], c2p[2];
	if(argc != 2){
		perror("Invalid input.\n");
		exit(1);
	}
	// Creating the pipes;
	pipe(p2c); pipe(c2p);

	if(fork() == 0){
		// working with the child
		// closing unused pipes
		close(p2c[1]); close(c2p[0]);
		//reading data from the parent to the child
		int i, n, sum=0, nr;
		double average;
		// reading the number of elements
		if(read(p2c[0], &n, sizeof(int)) < 0){
			perror("Error while reading from p2c.");
			close(p2c[0]); close(c2p[1]);
			exit(1);
		}
		// reading the elements
		for(i = 0; i < n; i++){
			if(read(p2c[0], &nr, sizeof(int)) < 0){
              perror("Error while reading from p2c.");
              close(p2c[0]); close(c2p[1]);
              exit(1);
         	}
			sum += nr;
			printf("Child received: %d \n", nr);
		}
		// computing the sum and returning to the parent the average.
		average = (double)sum/n;
		write(c2p[1], &average, sizeof(double));
		// closing the pipes.
		close(p2c[0]); close(c2p[1]);
		exit(1);
	}
	// Closing the unused ends in order to avoid deadlock
	close(p2c[0]); close(c2p[1]);
	
	int n = atoi(argv[1]);
	write(p2c[1], &n, sizeof(int));

	srandom(time(0));
	int rand_number, i;

	for(i = 0; i < n; i++){
		rand_number = random() % 100;
		printf("Parent generated: %d \n", rand_number);
		write(p2c[1], &rand_number, sizeof(int));
	}	

	double result;
	if(read(c2p[0], &result, sizeof(double)) > 0){
		printf("The average is: %f \n", result);
	}
	else{
		perror("Error reading the result.\n");
	}
		
	wait(0);
	close(p2c[1]); close(c2p[0]);
	return 0;
}
