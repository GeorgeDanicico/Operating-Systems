#include "common.h"

int main(int argc, char **argv){

        int fd_a2b = open(a2b, O_RDONLY);
        int fd_b2a = open(b2a, O_WRONLY);

        if(fd_a2b < 0 || fd_b2a < 0){
                perror("Error while opening the fifo.");
                exit(1);
        }




        close(fd_a2b); close(fd_b2a);
        
        return 0;
}
