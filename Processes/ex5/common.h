#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 200

char *a2b = "./a2b";
char *b2a = "./b2a";

void write_to_fd(int fd, char *buff){
	int length = strlen(buff);
	if(write(fd, &length, sizeof(int)) < 0){
		perror("Error while writing the length.");
		exit(1);
	}
	if(write(fd, buff, length * sizeof(char)) < 0){
		perror("Error while writing the string.");
		exit(1);
	}
}


void read_from_fd(int fd, int max, char *buff){
	int read_bytes = 0;
	while(read_bytes < max){
		int k;
		if((k = read(fd, buff + read_bytes, (max - read_bytes) * sizeof(char))) > 0){
			read_bytes += k;
		}
	}

}

