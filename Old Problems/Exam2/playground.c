// Using the following rules of the playground:


//     N kids spend their afternoons sharing their toys.
//     There are only M toys on the playground, one for every pair of kids.
//     In order to play, two kids share one and only one toy.
//     Kid A shares the toy with A+1, A being a even number.
//     A kid cannot play yet if another kid is already playing with the assigned toy.


// Implement using pthreads and pthreads mutex locks.


// Your program should print the kid that is playing, the toy that is using and when the kid finished playing.
// NOTE: if there is an odd number of kids, there will be a kid that will not share.


// NOTE: Code is compiled with gcc on a posix compliant operating system


// To ease coding on a IDE (VSCode) copy all code from the answer box and paste it on IDE text editor. To compile run build task (Terminal>Run Build Task...). To run the code open a new terminal (Terminal>New Terminal) if there isn't one already open and type ./prog <arg> (substitute <arg> to number of kids to test your code).


// The output should look like this: (remember the order of the lines is not enforced)


// Kid 0 is playing with toy 0
// Kid 0 finished playing
// Kid 1 is playing with toy 0
// Kid 1 finished playing

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct info {
    int kid;
    int toy;
};

pthread_t* th;
pthread_mutex_t* locks;
struct info* kidInfo;

void* routine(void* p) {
    struct info t = *(struct info*) p;
    pthread_mutex_lock(&locks[t.toy]);
    printf("Kid %d is playing with toy %d\n", t.kid, t.toy);
    sleep(1);
    printf("Kid %d finished playing\n", t.kid);
    pthread_mutex_unlock(&locks[t.toy]);
    return 0;
}

int main(int argc, char *argv[]) {
    // # of kids = # of threads
    int kids = atoi(argv[1]);

    // # of toys = # of locks
    int toys = kids/2;

    th = (pthread_t*) malloc(kids*sizeof(pthread_t));
    locks = (pthread_mutex_t*) malloc(toys*sizeof(pthread_mutex_t));
    kidInfo = (struct info*) malloc(kids*sizeof(struct info));

    for (int i = 0; i < toys; i++){
        pthread_mutex_init(locks+i, NULL);
    }

    for (int i = 0; i < kids; i++){
        kidInfo[i].kid = i;
        kidInfo[i].toy = i/toys;
        if (pthread_create(&th[i], NULL, &routine, (void*) &kidInfo[i]) != 0 ){
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < kids; i++){
        if(pthread_join(th[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }

    for (int i = 0; i < toys; i++){
        pthread_mutex_destroy(locks+i);
    }

    free(th);
    free(locks);
    free(kidInfo);

    return 0;
}