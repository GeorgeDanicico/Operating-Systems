#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 200


int main(){

	
	int p2c[2];

	if(pipe(p2c) < 0){
		perror("Error creating the pipes.");
		exit(1);
	}

	if(fork() == 0){
		close(p2c[1]);
		// reading from parent to child
		char* buffer;
		while(1){
		int k;
		if(read(p2c[0], &k, sizeof(int)) < 0){
              perror("Error while reading from parent to child.");
              close(p2c[0]);
              exit(1);
        }
		buffer = malloc(k*sizeof(char));
      
		if(read(p2c[0], buffer, k*sizeof(char)) < 0){
			perror("Error while reading from parent to child.");
			close(p2c[0]);
            exit(1);
			}

		if(strcmp(buffer, "stop") == 0)
			break;
		
		int i=0, ok=0;
		for(i=0; i<k && ok==0; i++)
			if(buffer[i] <'0' || buffer[i] >'9')
				ok=1;
		
		if(ok == 0)
			printf("%s \n", buffer);
		}
		free(buffer);
		close(p2c[0]);
		exit(0);
	}
	
	close(p2c[0]);
	char *buffer;
	while(1){
	int k;
	buffer=malloc(SIZE*sizeof(char));
	memset(buffer, 0, SIZE*sizeof(char));
	if((k=read(0,buffer,SIZE*sizeof(char))) < 0){
		perror("Error");
	}
	
	k--;
	if(write(p2c[1], &k, sizeof(int)) < 0)
		perror("Error.");
	buffer[k]=0;
	if(write(p2c[1], buffer, k*sizeof(char)) < 0){
		perror("Error while writing from parent to child.");
	}
	if(strcmp(buffer, "stop") == 0)
		{free(buffer);break;}

	free(buffer);
	}
	
	close(p2c[1]);
	wait(0);
	
	return 0;

}
