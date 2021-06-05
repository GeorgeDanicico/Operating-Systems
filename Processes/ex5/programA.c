#include "common.h"

void make_fifo(char *name){
	if(mkfifo(name, 0600)){
		perror("Error creating the fifo.");
		exit(1);
	}
}

int main(int argc, char **argv){
	
	make_fifo(a2b);
	make_fifo(b2a);

	int fd_a2b = open(a2b, O_WRONLY);
	int fd_b2a = open(b2a, O_RDONLY);

	if(fd_a2b < 0 || fd_b2a < 0){
		perror("Error while opening the fifo.");
		exit(1);
	}
	
	FILE *f;
	char *com = malloc(SIZE * sizeof(char));	
	char *buffer = malloc(SIZE * sizeof(char));
	memset(com, 0, SIZE * sizeof(char));
	memset(buffer, 0, SIZE * sizeof(char));


	while(1){
		int k;
		if((k = read(0, com, SIZE*sizeof(char))) < 0){
			perror("Error while reading the command.");
		}
		com[k-1]=0;
		if(strcmp(com, "stop") == 0){
			break;
		}


		if((f = popen(com, "r")) == NULL){
			perror("Error on popen");
		}
		else{
			while(fread(buffer, 1, SIZE-1, f) > 0){
				write_to_fd(fd_a2b, buffer);
				memset(buffer, 0, SIZE * sizeof(char));
			} 
		}
		pclose(f);
		memset(com, 0, SIZE * sizeof(char));
	}

	int stop = -1;
	if(write(fd_a2b, &stop, sizeof(int)) < 0){ perror("Error while sending stop.");
	}	


	close(fd_a2b); close(fd_b2a);
	free(com); free(buffer);
	if(unlink(a2b) < 0){

		perror("Error unlinking.");
	}	
	if(unlink(b2a) < 0){
        perror("Error unlinking.");
    }
	return 0;
}
