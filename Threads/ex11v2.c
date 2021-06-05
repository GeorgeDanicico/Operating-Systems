#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int *letters, *digits, *special;
	char* current;
	pthread_mutex_t* m;
}data;

void *func(void* a){

	data d = *((data*)a);
	int l=0, D=0, s=0,i;
	
	for(i=0; i<strlen(d.current); i++){
		if((d.current[i] >= 'a' && d.current[i] <= 'z') || (d.current[i] >= 'A' && d.current[i] <= 'Z'))
		l++;
		else if(d.current[i] >= '0' && d.current[i] <= '9')
			D++;
		else s++;
	}

	pthread_mutex_lock(d.m);
	*(d.letters) += l;
	*(d.digits) += D;	
	*(d.special) += s;
	pthread_mutex_unlock(d.m);

	return NULL;
}

int main(int argc, char** argv){
	int i;
	pthread_t t[argc+1];
	pthread_mutex_t m;
	data* d = malloc(sizeof(data) * argc);
	int *letters = malloc(sizeof(int));
	int *special = malloc(sizeof(int));
	int *digits = malloc(sizeof(int));	
	
	*letters=0;*special=0;*digits=0;
	pthread_mutex_init(&m, NULL);
	
	for(i=1; i<argc; i++){
		d[i].m = &m;
		d[i].letters = letters;
		d[i].special = special;
		d[i].digits = digits;
	}	

	for(i=1; i<argc; i++){
		d[i].current = argv[i];
		pthread_create(&t[i], NULL, func, (void*)(d+i));
	}

	for(i=1; i<argc; i++){
		pthread_join(t[i], NULL);
	}
	pthread_mutex_destroy(&m);
	
	printf("Letters: %d, Digits: %d, Specials: %d. \n",*(letters), *(digits), *(special));

	free(letters); 
	free(digits);	free(special);

	free(d);


}
