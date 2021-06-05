#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t m;
struct word {
	char* current_string;
	int* length;
};

void* func(void *a){
	pthread_mutex_lock(&m);
	struct word w = *((struct word*) a);	
	int i;
	for(i=0; i<*(w.length); i++){
		if(w.current_string[i] >= 97 && w.current_string[i] <= 122){
			w.current_string[i] -= 32;
		}
	}
	printf("%s \n", w.current_string);
		
	pthread_mutex_unlock(&m);
	return NULL;
}

int main(int argc, char** argv){
	pthread_t t[argc+1];
	int i;
	
	pthread_mutex_init(&m, NULL);
	struct word *w = malloc(sizeof(struct word) * argc);
	for(i=0; i<argc-1; i++){
		w[i].length = malloc(sizeof(int));
	}

	for(i=1; i<argc; i++){
		*(w[i-1].length) = strlen(argv[i]);
		w[i-1].current_string = argv[i];
		pthread_create(&t[i], NULL,func, (void*)(w+i-1));
	}	
	
	for(i=1; i<argc; i++){
        pthread_join(t[i], NULL);
    }
	
	for(i=0; i<argc-1; i++){
        free(w[i].length);
    }
	free(w);
	pthread_mutex_destroy(&m);


	return 0;
}
