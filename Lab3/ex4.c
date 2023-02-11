#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 20
int count = 0;
int front = 0, rear = 0; // producer and consumer indices
int buffer [BUFFER_SIZE]; // implemented like a circular buffer
pthread_t tid;
sem_t sem;  // To lock the critical code
sem_t inserted, available; // Using semaphores instead of whiles to check whether we can insert into the buffer

void insert(int item){
   // Will insert as long as we have an available spot in the buffer. Else, wait for spot to open up.
   sem_wait(&available);
   buffer[front] = item;
   front = (front + 1) % BUFFER_SIZE;  // Wraps the index around the circular buffer
   count++;
   sleep(1);
   sem_post(&inserted); // Increase the number of items inserted
}

int remove_item(){
   // Wait for an item to be inserted before we can remove it
   sem_wait(&inserted);
   int item;
   item = buffer[rear];
   rear = (rear + 1) % BUFFER_SIZE;
   count--;
   sleep(1);
   sem_post(&available); // Increase the number of available spots
   return item;
}

void * producer(void * param){
   int item;
   while(1){
      item = rand() % BUFFER_SIZE;
      // Lock critical data
      sem_wait(&sem);
      insert(item);
      sem_post(&sem);
      printf("in: %d inserted: %d\n", front, item);
   }
}

void * consumer(void * param){
   int item;
   while(1){
      sem_wait(&sem);
   	item = remove_item();
      sem_post(&sem);
   	printf("out: %d removed: %d\n", rear, item);
   }
}


int main(int argc, char * argv[])
{
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;

    // Initialize the semaphores
    sem_init(&sem, 0, 1);  // Initialize at one to let a thread through
    sem_init(&inserted, 0, 0); // Keeps track of items inserted
    sem_init(&available, 0, BUFFER_SIZE); // Keeps track of any available spots in the buffer (decreases each time)

    for (i = 0; i < producers; i++)
        pthread_create(&tid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
        pthread_create(&tid, NULL, consumer,  NULL);

    pthread_join(tid, NULL);
    sem_destroy(&sem);
    sem_destroy(&inserted);
    sem_destroy(&available);
}