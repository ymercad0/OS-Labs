#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef struct {
    int value;
} sempahore;

pthread_mutex_t sem_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sem_cond = PTHREAD_COND_INITIALIZER;

int sem_wait(sempahore* sem);
int sem_signal(sempahore* sem);

void* producer(void* param);
void* consumer(void* param);

int buffer [BUFFER_SIZE];
int front = 0;
int rear = 0;

sempahore mutex;
sempahore full;
sempahore empty;

pthread_t tid;

int sem_wait(sempahore* sem){
    pthread_mutex_lock(&sem_mutex);
    sem->value = sem->value - 1;
    if(sem->value < 0)
        pthread_cond_wait(&sem_cond, &sem_mutex);
    pthread_mutex_unlock(&sem_mutex);
    return 0;
}

int sem_signal(sempahore* sem){
    pthread_mutex_lock(&sem_mutex);
    sem->value = sem->value + 1;

    if(sem->value <= 0)
        pthread_cond_signal(&sem_cond);
    pthread_mutex_unlock(&sem_mutex);
    return 0;
}

void insert(int item){
    sem_wait(&empty); //If buffer is full, block producer
    sem_wait(&mutex); //Lock critical code

    buffer[front] = item;
    printf("front: %d, inserted: %d\n", front, item);
    front = (front + 1) % BUFFER_SIZE;
    sleep(1);

    sem_signal(&mutex); //Release critical code
    sem_signal(&full); //There is one space less in the buffer

}

int remove_item(){
    int item;
    sem_wait(&full); //If buffer is empty, block consumer
    sem_wait(&mutex); //Lock critical code

    item = buffer[rear];
    printf("rear: %d, removed: %d\n", rear, item);
    rear = (rear + 1) % BUFFER_SIZE;
    sleep(1);

    sem_signal(&mutex); //Release critical code
    sem_signal(&empty); //There is one space more available in the buffer

    return item;
}

void* producer(void* param){
    int item;
    while(1){
        item = rand() % BUFFER_SIZE;
        insert(item);
    }
}

void* consumer(void* param){
    int item;
    while(1){
        item = remove_item();
    }
}

int main(int argc, char * argv[]) {
    printf("Executing Producer/Consumer using Semaphores\n");
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;

    mutex.value = 1;
    full.value = 0;
    empty.value = BUFFER_SIZE;

    for (i = 0; i < producers; i++)
        pthread_create(&tid, NULL, producer,NULL);

    for (i = 0; i < consumers; i++)
        pthread_create(&tid, NULL, consumer, NULL);

    pthread_join(tid,NULL);


    return 0;
}



