#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t tid;
pthread_attr_t attr;
int *array;
void* fibonacci(void*);

void* fibonacci(void* param){
    int n = *((int*) param);
    if(n == 0) array[0] = 0;
    else if(n == 1) array[1] = 1;
    else array[n] = array[n - 2] + array[n - 1];
    pthread_exit(0);
}

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Did not provide input number");
        return -1;
    }

    if(atoi(argv[0]) < 0){
        fprintf(stderr,"Argument %d must be non-negative\n", atoi(argv[1]));
        return -1;
    }

    int n = atoi(argv[1]);
    array = malloc(sizeof(int) * n);
    pthread_attr_init(&attr);

    for (int i = 0; i <= n; i++) {
        pthread_create(&tid, &attr, &fibonacci, &i);
        pthread_join(tid, NULL);
    }

    for (int i = 0; i <= n; ++i) {
        printf("%d ", array[i]);
    }
    return 0;
}