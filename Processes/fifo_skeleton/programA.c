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


	

	close(fd_a2b); close(fd_b2a);
	if(unlink(a2b) < 0){
		perror("Error unlinking.");
	}	
	if(unlink(b2a) < 0){
        perror("Error unlinking.");
    }
	return 0;
}
