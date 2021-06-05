#include "common.h"

int main(int argc, char **argv){

    int fd_a2b = open(a2b, O_RDONLY);
    int fd_b2a = open(b2a, O_WRONLY);

    if(fd_a2b < 0 || fd_b2a < 0){
         perror("Error while opening the fifo.");
         exit(1);
    }

	while(1){
		int length;
		if(read(fd_a2b, &length, sizeof(int)) < 0){
			perror("Error while reading.");
			exit(1);
		}
		if(length == -1){
		break;
		}
		int i;
		char *buffer = malloc((length+1)*sizeof(char));
		read_from_fd(fd_a2b, length, buffer);

		for(i = 0; i < length; i++){
       		if(buffer[i] >= 'a' && buffer[i] <= 'z')
				buffer[i] -= 32;
		}

		buffer[length] = 0;
		write_to_fd(fd_b2a, buffer);
		free(buffer);

	}		



    close(fd_a2b); close(fd_b2a);
        
        return 0;
}
