#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	pthread_mutex_t m;
	int *letters, *digits, *special;
	char* current_string;
}data;
void *compute(void *a){
	data d = *((data *) a);
	int l=0, D=0, s=0, i;

	for(i=0; i<strlen(d.current_string); i++){
		if(d.current_string[i] >= '0' && d.current_string[i] <= '9')
			D++;
		else if(d.current_string[i] >= 'a' && d.current_string[i] <= 'z'){
			l++;
		}
		else s++;
	}

	pthread_mutex_lock(&d.m);
	
	*(d.letters) += l;
	*(d.digits) +=D;
	*(d.special) += s;	
	pthread_mutex_unlock(&d.m);

	return NULL;
}


int main(int argc, char** argv){
	
	int i;
	data *d = malloc(sizeof(data));
	pthread_t t[argc+1];
	pthread_mutex_init(&d->m, NULL);	
	d->letters = malloc(sizeof(int));d->special = malloc(sizeof(int));d->digits = malloc(sizeof(int));
	*(d->letters) = 0;*(d->special) = 0;*(d->digits) = 0;

	for(i = 1; i<argc; i++){
		d->current_string = argv[i];
		pthread_create(&t[i], NULL, compute, (void*)d);
	}	
	
	for(i = 1; i<argc; i++){
        pthread_join(t[i], NULL);
    }

	printf("The number of letters, digits, specials: ");
	printf("%d, %d, %d \n", *(d->letters), *(d->digits), *(d->special));

	pthread_mutex_destroy(&d->m);
	free(d->letters);
    free(d->special);
	free(d->digits);
	free(d);
	return 0;
}
