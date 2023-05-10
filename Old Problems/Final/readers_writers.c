#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_t tid;
pthread_mutex_t mutex;
pthread_mutex_t rw_mutex;
int read_count = 0;

void* readers(void* param){
    do{
        pthread_mutex_lock(&mutex);
        read_count++;
        if(read_count == 1) pthread_mutex_lock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        printf("Reader %d is reading\n", *((int*) param));
        sleep(1);

        pthread_mutex_lock(&mutex);
        read_count--;
        if(read_count == 0) pthread_mutex_unlock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

    }while(1);
}

void* writers(void* param){
    do{
        pthread_mutex_lock(&rw_mutex);

        printf("Writer %d is writing\n", *((int*) param));
        sleep(5);

        pthread_mutex_unlock(&rw_mutex);
    }while(1);
}

int main(int argc, char** argv){

    if(argc != 3) {
        fprintf(stderr, "Did not provide input readers/writers");
        return -1;
    }

    if(atoi(argv[1]) < 0 && atoi(argv[2]) < 0) {
        fprintf(stderr,"Argument %d & %d must be non-negative\n", atoi(argv[1]), atoi(argv[2]));
        return -1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    for (int i = 0; i < n; i++) {
        pthread_create(&tid, NULL, &readers, &i);
    }

    for (int i = 0; i < m; i++) {
        pthread_create(&tid, NULL, &writers, &i);
    }

    pthread_join(tid, NULL);

    return 0;
}