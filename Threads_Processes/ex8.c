#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv){
	
	int p2c[2],c2p[2];
	pipe(p2c);pipe(c2p);
	srandom(getpid());
	if(fork()==0){
		close(p2c[1]);close(c2p[0]);
		int* dig = malloc(sizeof(int)*16);
		read(p2c[0], dig, sizeof(int)*16);
		int i, nr=0, power=1;
		for(i=0; i<16; i++){
			nr = nr + power*dig[i];
			power*=2;
		}		
		write(c2p[1], &nr, sizeof(int));

		close(p2c[0]);close(c2p[1]);
		free(dig);
		exit(0);
	}
	int* digits = malloc(sizeof(int)*16);
	int i, nr;
	printf("The parent generated: ");
	for(i=0; i<16; i++){
		digits[i] = random() % 2;
		printf("%d ", digits[i]);
	}
	printf("\n");
	close(p2c[0]);close(c2p[1]);
	write(p2c[1], digits, sizeof(int)*16);
	read(c2p[0], &nr, sizeof(int));
	wait(0);

	close(p2c[1]);close(c2p[0]);
	printf("The number is: %d.\n", nr);
	free(digits);

	return 0;
}
