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
	srandom(time(0));
	if(fork() == 0){
		close(p2c[0]); close(c2p[1]);
		// reading from parent to child
		int n, i;
		read(c2p[0], &n, sizeof(int));
		write(p2c[1], &n, sizeof(int));
		for(i = 0; i<n;i++){
			int var;
			var = random() % 51;
			printf("Program D generated: %d \n", var);
			write(p2c[1], &var, sizeof(int));
		}
		close(p2c[0]); close(c2p[1]);
		exit(0);
	}

	if(fork() == 0){
		// program B
		close(p2c[1]); close(c2p[0]);
		if(fork() == 0){
			// Program C
			int nr = random() % 4 + 2;	
			printf("Program C generated: %d \n", nr);
			write(c2p[1], &nr, sizeof(int));
			close(c2p[1]);	
			exit(0);
		}
		int n,i,sum=0,nr;
		read(p2c[0], &n, sizeof(int));
		for(i = 0;i<n;i++){
			read(p2c[0], &nr, sizeof(int));
			sum+=nr;
		}
		printf("The sum is: %d \n", sum);

		close(p2c[0]); close(c2p[1]);

		wait(0);
		exit(0);

	}


	close(p2c[0]); close(c2p[1]);
	close(p2c[1]); close(c2p[0]);

	wait(0);
	wait(0);	
	return 0;

}
