#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(){

	
	int p2c[2], c2p[2];

	if(pipe(p2c) < 0 || pipe(c2p) < 0){
		perror("Error creating the pipes.");
		exit(1);
	}

	if(fork() == 0){
		close(p2c[1]); close(c2p[0]);
		// reading from parent to child
		int buffer=5;
		while(buffer >= 5){
		if(read(p2c[0], &buffer, sizeof(int)) < 0){
			perror("Error while reading from parent to child.");
			close(p2c[0]); close(c2p[1]);
            exit(1);
		}
		printf("Program B received: %d \n", buffer);
		buffer/=2;
		printf("Program B sends: %d \n", buffer);

		// writing from the child to parent
		if(write(c2p[1], &buffer, sizeof(int)) < 0){
            perror("Error while writing from child to parent.");
            close(p2c[0]); close(c2p[1]);
            exit(1);
        }
}
		close(p2c[0]); close(c2p[1]);
		exit(0);
	}

	close(p2c[0]); close(c2p[1]);
	int buffer;
	srandom(time(0));
	buffer = random() % 151 + 50;
	printf("Number generated: %d \n", buffer);
	while(buffer >= 5){
		if(buffer % 2 == 1)	
			buffer ++;		

		printf("Program A sends: %d \n", buffer);
		if(write(p2c[1], &buffer, sizeof(int)) < 0){
			perror("Error while writing from parent to child.");
		}

	
		if(read(c2p[0], &buffer, sizeof(int)) < 0){
			perror("Error while reading from child to parent.");
		}
		printf("Program A received: %d \n", buffer);
}
	
	close(p2c[1]); close(c2p[0]);
	wait(0);
	
	return 0;

}
