#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv){

	int c1_to_c2[2], c2_to_c1[2];
	// creating the pipes.
	pipe(c1_to_c2); pipe(c2_to_c1);

	srandom(time(0));
	if(fork() == 0){
		// closing the unused pipes.
		close(c1_to_c2[0]); close(c2_to_c1[1]);	
		while(1){
			int rand_nr = random() % 10 + 1;
			
			printf("Child 1 generated: %d.\n", rand_nr);
		
			write(c1_to_c2[1], &rand_nr, sizeof(int));
			if(read(c2_to_c1[0], &rand_nr, sizeof(int)) < 0){
				perror("Error at reading from child2 to child1.\n");
			}
			if(rand_nr == 10){
				printf("Stop.\n");
				break;
			}
		}

		close(c1_to_c2[1]); close(c2_to_c1[0]);
		exit(1);
	}


	if(fork() == 0){

		close(c1_to_c2[1]); close(c2_to_c1[0]);
        while(1){
     	 	int rand_nr;
            if(read(c1_to_c2[0], &rand_nr, sizeof(int)) < 0){
	             perror("Error at reading from child2 to child1.\n");
            }
            if(rand_nr == 10){
                printf("Stop.\n");
                break;
           	} 
			rand_nr = random() % 10 + 1;
			
			printf("Child 2 generated: %d.\n", rand_nr);
			write(c2_to_c1[1], &rand_nr, sizeof(int));			
       }
 
       close(c1_to_c2[0]); close(c2_to_c1[1]);
       exit(1);
	}
		
	close(c1_to_c2[1]); close(c2_to_c1[0]);
	close(c1_to_c2[0]); close(c2_to_c1[1]);
	wait(0);
	wait(0);

	return 0;
}
