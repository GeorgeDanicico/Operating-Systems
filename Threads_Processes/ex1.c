#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


void* func(void* a){
	return NULL;
}

int main(int argc, char** argv){

	int p2c[2],c2p[2];
	
	srandom(time(NULL));
	pipe(p2c);pipe(c2p);
	if(fork()==0){
		close(p2c[1]);close(c2p[0]);
		int n,i;
		char* c;
		read(p2c[0], &n, sizeof(int));
		c = malloc(sizeof(char)*(n+1));
		read(p2c[0], c, sizeof(char) * n);

		for(i=0; i<n; i++){
			int rn = random() % 25;
			c[i] = c[i] + rn;
		}

		write(c2p[1], &n, sizeof(int));
        write(c2p[1], c, sizeof(char) * n);
		
		close(p2c[0]);close(c2p[1]);
		free(c);
		exit(1);
	}


	int n,i;
	n = random()%21 + 10;
	char* characters = malloc((n+1)*sizeof(char));
	for(i=0; i<n;i++){
		characters[i] = 'a';
	}
	close(p2c[0]);close(c2p[1]);
	write(p2c[1], &n, sizeof(int));
	write(p2c[1], characters, sizeof(char) * n);
	
	read(c2p[0], &n, sizeof(int));
    read(c2p[0], characters, sizeof(char) * n);

	close(p2c[1]);close(c2p[0]);
	wait(0);
	printf("The length is %d and string is: ", n);
	for(i=0; i<n; i++)
		printf("%c",characters[i]);
	printf("\n");

	free(characters);

	return 0;
}
