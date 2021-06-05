#include "common.h"

int main(int argc, char **argv){

        int fd_a2b = open(a2b, O_RDONLY);
        int fd_b2a = open(b2a, O_WRONLY);

        if(fd_a2b < 0 || fd_b2a < 0){
                perror("Error while opening the fifo.");
                exit(1);
        }
		
		int n,i;
		if (read(fd_a2b, &n, sizeof(int)) < 0) perror("Error");

		int* v= malloc(n*sizeof(int));

if (read(fd_a2b, v, n*sizeof(int)) < 0) perror("Error");
		int count=0;
		for(i=0;i<n;i++){
			if(v[i] % 3!=0)
				count++;
		}

		write(fd_b2a, &count, sizeof(int));

        close(fd_a2b); close(fd_b2a);
        
        return 0;
}
