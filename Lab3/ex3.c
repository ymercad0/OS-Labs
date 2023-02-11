#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 20
int count = 0;
int front = 0, rear = 0; // producer and consumer indices
int buffer [BUFFER_SIZE]; // implemented like a circular buffer
pthread_t tid;
pthread_mutex_t mutex;

void insert(int item){
   while ((rear - front + BUFFER_SIZE) % BUFFER_SIZE == BUFFER_SIZE); // Don't add more items than the buffer can hold
   buffer[front] = item;
   front = (front + 1) % BUFFER_SIZE;  // Wraps the index around the circular buffer
   count++;
   sleep(1);
}

int remove_item(){
   int item;
   while ((rear - front + BUFFER_SIZE) % BUFFER_SIZE == 0);  // Can't remove from an empty buffer
   item = buffer[rear];
   rear = (rear + 1) % BUFFER_SIZE;
   count--;
   sleep(1);
   return item;
}

void * producer(void * param){
   int item;
   while(1){
      item = rand() % BUFFER_SIZE;
      // Mutex Lock prevents threads from accessing shared buffer
      pthread_mutex_lock(&mutex);
      insert(item);
      pthread_mutex_unlock(&mutex);
      printf("in: %d inserted: %d\n", front, item);
   }
}

void * consumer(void * param){
   int item;
   while(1){
      // Mutex Lock
      pthread_mutex_lock(&mutex);
   	item = remove_item();
      pthread_mutex_unlock(&mutex);
   	printf("out: %d removed: %d\n", rear, item);
   }
}


int main(int argc, char * argv[])
{
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < producers; i++)
        pthread_create(&tid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
        pthread_create(&tid, NULL, consumer,  NULL);

    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mutex);
}