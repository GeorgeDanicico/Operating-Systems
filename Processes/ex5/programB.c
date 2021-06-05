#include "common.h"

int main(int argc, char **argv){

        int fd_a2b = open(a2b, O_RDONLY);
        int fd_b2a = open(b2a, O_WRONLY);

        if(fd_a2b < 0 || fd_b2a < 0){
                perror("Error while opening the fifo.");
                exit(1);
        }
		
	char *buf = (char*)malloc(SIZE*sizeof(char));
    // init allocated space with 0
    memset(buf, 0, SIZE * sizeof(char));

		while(1){
			int length;
			if(read(fd_a2b, &length, sizeof(int)) < 0){
				perror("Error reading");
			}
			if(length == -1){ break; }

			int i = 0, k;
			while( i < length ){
				int size = (length - i > SIZE -1) ? SIZE - 1  : length  - i;
				if((k = read(fd_a2b, buf, size * sizeof(char))) < 0){ perror("Error."); } 
				else{
					i += k;
					printf("%s", buf);
					memset(buf, 0, SIZE * sizeof(char));
				}
			}
		}
        close(fd_a2b); close(fd_b2a);
        return 0;
}
