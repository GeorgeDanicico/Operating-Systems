#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){

	int a2d[2], b2d[2], c2d[2], d2b[2], d2c[2];

	pipe(a2d);pipe(b2d);pipe(c2d);pipe(d2c);pipe(d2b);

	if(fork()==0){
		srandom(getpid());
		int stop=1;
		close(a2d[1]);close(b2d[0]);close(d2b[1]);close(d2c[1]);close(c2d[0]);
		close(a2d[0]);close(c2d[1]);close(d2c[0]);
		
		while(stop == 1){
			int nr = random()%200 + 1;
			printf("B generated %d.\n", nr);
			write(b2d[1], &nr, sizeof(int));
			read(d2b[0], &stop, sizeof(int));
		}
		close(b2d[1]);close(d2b[0]);
		exit(1);
	}

	if(fork()==0){
		srandom(getpid());
		int stop=1;
		close(a2d[1]);close(b2d[1]);close(d2b[0]);close(d2c[1]);close(c2d[0]);
		close(a2d[0]);close(b2d[0]);close(d2b[1]);
		while(stop == 1){
			int nr = random()%200 + 1;
			printf("C generated %d.\n", nr);
			write(c2d[1], &nr, sizeof(int));
			read(d2c[0], &stop, sizeof(int));
		}
		close(c2d[1]);close(d2c[0]);
		exit(1);
	}

	if(fork()==0){
		int nr;
		close(a2d[1]);close(b2d[1]);close(d2b[0]);close(d2c[0]);close(c2d[1]);
		read(a2d[0], &nr, sizeof(int));
		
		while(1){
			int nrb, nrc;
			int sgn = 1;
			read(b2d[0], &nrb, sizeof(int));
			read(c2d[0], &nrc, sizeof(int));
			if(abs(nrb-nrc) <= nr){
				sgn = -1;
			}
			write(d2b[1], &sgn, sizeof(int));
			write(d2c[1], &sgn, sizeof(int));
		}		

		close(a2d[0]);close(b2d[0]);close(d2b[1]);close(d2c[1]);close(c2d[0]);
		exit(1);
	}
	
	
	close(a2d[0]);close(b2d[1]);close(b2d[0]);close(c2d[0]);close(c2d[1]);close(d2c[1]);
	close(d2c[0]);close(d2b[0]);close(d2b[1]);
	int nr = random() % 11 + 10;
	printf("A generated %d.\n", nr);
	write(a2d[1], &nr, sizeof(int));

	close(a2d[1]);

	wait(0);wait(0);wait(0);

	return 0;
}
