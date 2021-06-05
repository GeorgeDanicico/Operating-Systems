#include "common.h"

int main(int argc, char **argv){

        int fd_a2b = open(a2b, O_RDONLY);

        if(fd_a2b < 0){
                perror("Error while opening the fifo.");
                exit(1);
        }

		char *buffer = malloc(SIZE*sizeof(char));
		memset(buffer, 0, SIZE*sizeof(char));

		while(1){
			int k;
			if(read(fd_a2b, &k, sizeof(int)) < 0){
				perror("Error.");}
			if(k == -1)
				break;
			int count=0, i;
			read_from_fd(fd_a2b, k, buffer); 
			for(i = 0; i < k; i++){
				if(strchr("aeiouAEIOU", buffer[i]))
					count++;
			}
			printf("Program B counted: %d \n", count);
			memset(buffer, 0, SIZE*sizeof(char));
		}


        close(fd_a2b);
        free(buffer);
        return 0;
}
