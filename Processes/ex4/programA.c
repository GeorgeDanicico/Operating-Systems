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
	
	int size = 0, i;
	for(i = 1; i < argc; i++){
		size = size + strlen(argv[i]) + 1;
	}
	size ++;
	char *result = malloc(size * sizeof(char));
	memset(result, 0, size*sizeof(char));

	for(i = 1; i < argc; i++){
		write_to_fd(fd_a2b, argv[i]);

		int length;
		if(read(fd_b2a, &length, sizeof(int)) < 0){
			perror("Error while reading.");
			exit(1);
		}
		char *buffer = malloc((length+1)*sizeof(char));
		read_from_fd(fd_b2a, length, buffer);
		buffer[length]=0;
		strcat(result, buffer);
		strcat(result, " ");
		free(buffer);
	}
	int stop = -1;
	if(write(fd_a2b, &stop, sizeof(int)) < 0){
		perror("Error while sending stop.");
	}
	result[size - 1] = 0;
	printf("%s \n", result);
	free(result);

	close(fd_a2b); close(fd_b2a);
	if(unlink(a2b) < 0){
		perror("Error unlinking.");
	}	
	if(unlink(b2a) < 0){
        perror("Error unlinking.");
    }
	return 0;
}
