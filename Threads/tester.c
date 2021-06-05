#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

char* str;
int counter;
pthread_mutex_t m;

void* func(void *a){
    char *current = (char*) a;
    int i, notlower=0;

    for(i=0; i<strlen(current) && notlower == 0; i++){
        if(current[i] < 97 || current[i] > 122)
            notlower = 1;
    }
    pthread_mutex_lock(&m);
    if(notlower == 0){
        int newlength = 1 + strlen(str) + strlen(current);
        char *oldstring = malloc(sizeof(char) * newlength);
        strcpy(oldstring, str);
        strcat(oldstring, current);
        free(str);
        str = oldstring;
    }
    else{
        counter++;
    }
    pthread_mutex_unlock(&m);

    return NULL;
}

int main(int argc, char** argv){

    if(argc < 2){
        perror("Error");
        exit(1);
    }
    int i;
    str = malloc(sizeof(char)*1);
    str[0]=0;

    pthread_t t[argc+1];
    pthread_mutex_init(&m, NULL);
    for(i=1; i<argc; i++){

        pthread_create(&t[i], NULL, func, (void*)argv[i]);
    }

  
    for(i=1; i<argc; i++){
        pthread_join(t[i], NULL);
    }
	pthread_mutex_destroy(&m);
    printf("The global string is: %s.\n", str);
    printf("The global counter value is: %d.\n", counter);

    free(str);
    return 0;
}
