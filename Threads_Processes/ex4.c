#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv){
	
	int n = atoi(argv[1]);
	int p2a[2], p2b[2], a2p[2], b2p[2];
	pipe(p2b);pipe(a2p);pipe(b2p);pipe(p2a);
	
	if(fork() == 0){
		srandom(getpid());		
		close(p2a[1]);close(a2p[0]);close(b2p[1]);close(b2p[0]);close(p2b[1]);close(p2b[0]);
		int i, child=1;
		int *digits = malloc(sizeof(int)*10);
		read(p2a[0], digits, sizeof(int)*10);
		for(i=0; i<n; i++){
			int rn = random()%900 + 100;
			printf("Child %d generated %d.\n",child, rn);
			while(rn){
				digits[rn%10]++;
				rn/=10;
			}
		}
		printf("The frequency array of Child %d is: ", child);
		for(i=0; i<10; i++)
			printf("%d ", digits[i]);
		
        write(a2p[1], digits, sizeof(int)*10);

		close(p2a[0]);close(a2p[1]);
		printf("\n");
		free(digits);	
		exit(0);
	}

	if(fork()==0){
		srandom(getpid());
        close(p2b[1]);close(b2p[0]);close(a2p[1]);close(a2p[0]);close(p2a[1]);close(p2a[0]    );
        int i, child=2;
        int *digits = malloc(sizeof(int)*10);
        read(p2b[0], digits, sizeof(int)*10);
        for(i=0; i<n; i++){
            int rn = random()%900 + 100;
            printf("Child %d generated %d.\n",child, rn);
            while(rn){
                digits[rn%10]++;
                rn/=10;
            }
        }
        printf("The frequency array of Child %d is: ", child);
        for(i=0; i<10; i++)
            printf("%d ", digits[i]);

        write(b2p[1], digits, sizeof(int)*10);

        close(p2b[0]);close(b2p[1]);
 		
		printf("\n");
        free(digits);
		exit(0);
	}
	int i;
	int *freq11 = malloc(sizeof(int)*10);
    int *freq22 = malloc(sizeof(int)*10);
	for(i=0; i<10; i++)
		freq11[i]=0,freq22[i]=0;


	close(p2a[0]);close(a2p[1]);close(b2p[1]);close(p2b[0]);
	write(p2a[1], freq11, sizeof(int)*10);
    write(p2b[1], freq22, sizeof(int)*10);

	read(a2p[0], freq11, sizeof(int)*10);
	read(b2p[0], freq22, sizeof(int)*10);

	wait(0);wait(0);
	close(p2a[1]);close(a2p[0]);close(b2p[0]);close(p2b[1]);	

	for(i=0; i<10; i++){
		if(freq11[i] > freq22[i])
			printf("%d - Child 1.\n", i);
		else if(freq11[i] < freq22[i])
			printf("%d - Child 2.\n", i);
		else  printf("%d - equal.\n", i);
	}

	free(freq11);
	free(freq22);
	return 0;
}
