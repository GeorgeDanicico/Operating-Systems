#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char vowels[1000]="",digits[1000]="";
typedef struct{
	char* vowels;
	char* current_string;
	int *size;
}vow;

typedef struct{
	char* digits;
	char* current_string;
	int *size;
}dig;

pthread_mutex_t m1, m2;

void* func1(void *a){
	vow *v = (vow*)a;
	int i;
	for(i=0; i<strlen(v->current_string); i++){
		if(strchr("AEIOUaeiou", v->current_string[i])){
			pthread_mutex_lock(&m1);
			v->vowels[*v->size]=v->current_string[i];
			int N = *v->size;
            N++;
            *v->size = N;
			pthread_mutex_unlock(&m1);
		}
	}


	return NULL;
}

void *func2(void *a){
	dig *v = (dig*)a;
    int i;
	//printf("%s\n", v->current_string);
    for(i=0; i<strlen(v->current_string); i++){
        if(strchr("0123456789", v->current_string[i])){
           pthread_mutex_lock(&m2);
           v->digits[*v->size]=v->current_string[i];
			int N = *v->size;
			N++;
			*v->size = N;
			pthread_mutex_unlock(&m2);
        }
    }


	return NULL;
}


int main(int argc, char **argv){
	int a2p[2], b2p[2];

	pthread_mutex_init(&m1, NULL);
	pthread_mutex_init(&m2, NULL);

	pipe(a2p);pipe(b2p);
	int n = argc;
	if(fork() == 0){
		
		close(a2p[0]);close(b2p[0]);close(b2p[1]);
		int i;
		pthread_t t[n+1];
		
		vow *arg = malloc(sizeof(vow)*n);
		char* vows = malloc(sizeof(char)*100);
		int* nr = malloc(sizeof(int));
		*nr = 0;	
		for(i=1; i<n; i++){
			arg[i].size=nr;
			arg[i].current_string = argv[i];
			arg[i].vowels = vows;
			pthread_create(&t[i], NULL, func1, (void*)(arg+i));
		}

		for(i=1; i<n; i++){
			pthread_join(t[i], NULL);
		}
			
		write(a2p[1], nr, sizeof(int));
		write(a2p[1], vows, sizeof(char)*(*nr));


		close(a2p[1]);
		free(nr);
		free(vows);
		free(arg);
		exit(1);
	}

	if(fork()==0){
		close(a2p[0]);close(b2p[0]);close(a2p[1]);
		int i;
        pthread_t t[n+1];

         dig *arg = malloc(sizeof(dig)*n);
         char* D = malloc(sizeof(char)*100);
         int* nr = malloc(sizeof(int));
         *nr = 0;
         for(i=1; i<n; i++){
             arg[i].size=nr;
             arg[i].current_string = argv[i];
             arg[i].digits = D;
             pthread_create(&t[i], NULL, func2, (void*)(arg+i));
         }
 
         for(i=1; i<n; i++){
              pthread_join(t[i], NULL);
         }
 
         write(b2p[1], nr, sizeof(int));
         write(b2p[1], D, sizeof(char)*(*nr));
		close(b2p[1]);
         free(nr);
         free(D);
         free(arg);
		exit(1);
	}
	
	close(b2p[1]);close(a2p[1]);
	int i, n1, n2;
	read(b2p[0], &n1, sizeof(int));
	read(a2p[0], &n2, sizeof(int));

	char *d = malloc(sizeof(char)*n1);
	char *v = malloc(sizeof(char)*n2);

	read(a2p[0], v, sizeof(char)*n2);
	read(b2p[0], d, sizeof(char)*n1);

	wait(0);wait(0);	

	printf("The vowels are: ");
	for(i=0; i<n2; i++)
		printf("%c", v[i]);
	printf("\n");
	printf("The digits are: ");
    for(i=0; i<n1; i++)
         printf("%c",d[i]);
    printf("\n");
	
	close(b2p[1]);close(a2p[1]);
	
	return 0;
}


