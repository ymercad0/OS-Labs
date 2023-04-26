#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

// pthread_mutex_t* global_lock_ptr;
int MaxTime=5;

typedef struct {
    int idx;
    float N;
    char a_string[255];
    pthread_mutex_t* lock_ptr;
    sem_t *monitor;

} wargs_t;

typedef enum {
    WORK,
    SYMM,
    ASYM,
    MNTR
} workertype_t;

void* worker(void *param){
    wargs_t* argp;
    argp = (wargs_t *) param;
    printf("Message from Thread Spawner: %s\n", argp->a_string);
    printf("Acessing parameter idx struct: %d\n", argp->idx);
    pthread_exit(0);
}

void* philosopher_symm(void *param){
    wargs_t* argp;
    argp = (wargs_t *) param;
    printf("************ SYMMETRIC ******************\n");
    int i = argp->idx;
    int N = argp->N;
    pthread_mutex_t *lstick = &(argp->lock_ptr[i]);
    pthread_mutex_t *rstick = &(argp->lock_ptr[(i+1)%N]);

    int j=0;
    while(1) {
        printf("Philosopher %d is hungry for the %d time.\n",i,++j);
        pthread_mutex_lock(lstick);
        // printf("Philosopher %d, grabbed left chopstick\n",i);
        pthread_mutex_lock(rstick);
        // printf("Philosopher %d, grabbed right chopstick\n",i);

        printf("Philosopher %d is eating\n",i);
        usleep(rand()%MaxTime);
        pthread_mutex_unlock(lstick);
        // printf("Philosopher %d, released left chopstick\n",i);
        pthread_mutex_unlock(rstick);
        // printf("Philosopher %d, released right chopstick\n",i);
        
        printf("Philosopher %d is thinking\n",i);
        usleep(rand()%MaxTime);
        
    }
    pthread_exit(0);
}

void* philosopher_asym(void* param){
    wargs_t* argp;
    argp = (wargs_t*) param;
    printf("************ ASYMMETRIC ******************\n");
    int i = argp->idx;
    int N = argp->N;
    pthread_mutex_t *lstick = &(argp->lock_ptr[i]);
    pthread_mutex_t *rstick = &(argp->lock_ptr[(i+1)%N]);

    int j = 0;
    while(1){
        printf("Philosopher %d is hungry for the %d time.\n",i,++j);
        if(i%2) {
            pthread_mutex_lock(lstick);
            pthread_mutex_lock(rstick);
        }
        else {
            pthread_mutex_lock(rstick);
            pthread_mutex_lock(lstick);    
        }
        printf("Philosopher %d is eating\n",i);
        usleep(rand()%MaxTime);
        pthread_mutex_unlock(lstick);
        pthread_mutex_unlock(rstick);
        printf("Philosopher %d is thinking\n",i);
        usleep(rand()%MaxTime);
    }

    pthread_exit(0);
}

void* philosopher_monitor(void *param){
    wargs_t* argp;
    argp = (wargs_t *) param;

    int i = argp->idx;
    int N = argp->N;
    pthread_mutex_t *lstick = &(argp->lock_ptr[i]);
    pthread_mutex_t *rstick = &(argp->lock_ptr[(i+1)%N]);

    sem_t *one_monitor = argp->monitor;
    
    int j=0;
    while(1) {
        printf("Philosopher %d is hungry for the %d time.\n",i,++j);
        
        // Dont grap chostiks if too many philosophers have
        // reached for them.
        sem_wait(one_monitor);
        printf("Monitor allowed Philosopher %d to eat.\n",i);
        
        // Get ready to eat
        pthread_mutex_lock(lstick);
        pthread_mutex_lock(rstick);
        printf("Philosopher %d is eating\n",i);
        usleep((rand()%MaxTime*10));
        
        // Done eating, release resources
        pthread_mutex_unlock(lstick);
        pthread_mutex_unlock(rstick);
        // Increase the semaphore count
        sem_post(one_monitor);
        
        // Spend some time thinking.
        printf("Philosopher %d is thinking\n",i);
        usleep(rand()%MaxTime);
        
    }

    pthread_exit(0);
}

void thread_spawner(int N, workertype_t worker_type) {
    wargs_t* argp;
    pthread_t* thread_ptr;
    pthread_mutex_t* lock_ptr;
    // pthread attributes not need if not used
    // pthread_attr_t attr;
    // pthread_attr_t* attrp;
    sem_t *monitor;
    
    // global_lock_ptr = lock_ptr;
    argp = (wargs_t*) malloc(sizeof(wargs_t)*N);
    
    // Initialize Mutex Locks
    lock_ptr = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*N);
    for(int i = 0; i < N; i++){
        if (pthread_mutex_init(&lock_ptr[i], NULL) != 0) {
            printf("\n mutex init has failed\n");
            return;
        }
    }
    // Monitor with a counting semaphore
    monitor = (sem_t*)malloc(sizeof(sem_t));
    int max_eaters = N-1;
    sem_init(monitor, 0, max_eaters);

    // Populate paramenters and Create Threads
    thread_ptr = (pthread_t*) malloc(sizeof(pthread_t)*N);
    for(int i=0; i<N; i++) {
        argp[i].idx = i;
        argp[i].N = N;
        sprintf(argp[i].a_string,"Message to thread %d",i);
        argp[i].lock_ptr = lock_ptr;
        argp[i].monitor = monitor;

        switch(worker_type) {
            case WORK:
                pthread_create(&thread_ptr[i], NULL, worker, &argp[i]);
                break;
            case SYMM:
                pthread_create(&thread_ptr[i], NULL, philosopher_symm, &argp[i]);
                break;
            case ASYM:
                pthread_create(&thread_ptr[i], NULL, philosopher_asym, &argp[i]);
                break;
            case MNTR:
                pthread_create(&thread_ptr[i], NULL, philosopher_monitor, &argp[i]);
                break;
        }
    }

    // Join Threads
    for(int i=0; i<N; i++){
        pthread_join(thread_ptr[i],NULL);
    }

    // Free dynamically allocated memory
    free(thread_ptr);
    free(lock_ptr);
    free(argp);
}

int main(int argc, char **argv){
    // Get arguments from command line
    // and pass them to thread spawner

    // Get  Number of Cores
    int Nthreads=0;
    workertype_t worker_type;
    if(argc != 3) {
        printf("Wrong number of arguments: %d\n", argc-1);
        exit(-1);
    }
    else {
        Nthreads = atoi(argv[1]);
        printf("Number of threads set to: %d\n",Nthreads);
        if(!strcmp(argv[2],"work")) {
            worker_type = WORK;
        }
        else if(!strcmp(argv[2],"symm")) {
            worker_type = SYMM;
        }
        else if(!strcmp(argv[2],"asym")) {
            worker_type = ASYM;
        }
        else if(!strcmp(argv[2],"mntr")) {
            worker_type = MNTR;
        }
        else{
            printf("Unrecognized worker thread type (%s)\n",argv[2]);
            exit(-1);
        }
    }

    thread_spawner(Nthreads,worker_type);
    
    exit(0);
}
