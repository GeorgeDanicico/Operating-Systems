#include "common.h"
#include<signal.h>
void make_fifo(char *name){
	if(mkfifo(name, 0600)){
		perror("Error creating the fifo.");
		exit(1);
	}
}

void handler(int sig){
	printf("\nSIGINT received.\n");
	unlink(a2b);
	exit(1);
}

int main(int argc, char **argv){
	
	make_fifo(a2b);

	signal(SIGINT, handler);

	int fd_a2b = open(a2b, O_WRONLY);

	if(fd_a2b < 0){
		perror("Error while opening the fifo.");
		exit(1);
	}
	
	char *buffer = malloc(SIZE*sizeof(char));
	memset(buffer, 0, SIZE*sizeof(char));

	while(1){
		
		int k;
		if((k=read(0, buffer, SIZE*sizeof(char))) < 0)
			{perror("Error");}

		buffer[k-1]=0;
		printf("----%d \n", k);			
		if(strcmp(buffer, "stop") == 0){
			break;
		}
        k--;
		write(fd_a2b, &k, sizeof(int));
		write(fd_a2b, buffer, k*sizeof(char));
			
		memset(buffer, 0, SIZE*sizeof(char));
	}
	int stop = -1;
	if(write(fd_a2b, &stop, sizeof(int)) < 0)
		perror("Error sending the stop.");
	
	close(fd_a2b);
	free(buffer);
	if(unlink(a2b) < 0){
		perror("Error unlinking.");
	}	
	return 0;
}
