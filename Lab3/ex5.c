#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 20
int count = 0;
int front = 0, rear = 0; // producer and consumer indices
int buffer [BUFFER_SIZE]; // implemented like a circular buffer
pthread_t tid;
// A mutex + conditional threads == a monitor
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // for statically allocated mutexes
pthread_cond_t notFull, notEmpty;

void insert(int item){
   // Don't add more items than the buffer can hold
   while ((rear - front + BUFFER_SIZE) % BUFFER_SIZE == BUFFER_SIZE) pthread_cond_wait(&notFull, &mutex);
   buffer[front] = item;
   front = (front + 1) % BUFFER_SIZE;  // Wraps the index around the circular buffer
   count++;
   sleep(1);
   pthread_cond_signal(&notEmpty);  // An item has been added, buffer is most definitely not empty
}

int remove_item(){
   // Can't remove from an empty buffer
   int item;
   while ((rear - front + BUFFER_SIZE) % BUFFER_SIZE == 0) pthread_cond_wait(&notEmpty, &mutex);
   item = buffer[rear];
   rear = (rear + 1) % BUFFER_SIZE;
   count--;
   sleep(1);
   pthread_cond_signal(&notFull);  // Item has been removed, not full anymore
   return item;
}

void * producer(void * param){
   int item;
   while(1){
      item = rand() % BUFFER_SIZE;
      insert(item);
      printf("in: %d inserted: %d\n", front, item);
   }
}

void * consumer(void * param){
   int item;
   while(1){
   	item = remove_item();
   	printf("out: %d removed: %d\n", rear, item);
   }
}


int main(int argc, char * argv[])
{
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;

    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);

    for (i = 0; i < producers; i++)
        pthread_create(&tid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
        pthread_create(&tid, NULL, consumer,  NULL);

    pthread_join(tid, NULL);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);
}