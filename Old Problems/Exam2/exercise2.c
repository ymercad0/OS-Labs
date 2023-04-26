// Using the following rules of the construction site:

//     N construction workers are taking turns to move stones.
//     There are the same amount of stones as construction workers.
//     In order to take turns first any worker moves 2 stones.
//     Secondly, a worker that already moved 2 stones can only then move a single stone.
//     The only stones that worker A can move are stones A-1, A, and A+1. ('A' being a number greater or equal than 0).
//     There are not a stone numbered less than 0 and greater than N.
//     The worker can move any random stones near them.
//     If a worker has not moved any stone they must wait for two available stones to move at the same time.
//     If a worker has already moved their first two stones and there isn't any single stone to move they must wait until there is an available stone.
//     Any worker finished their job if they completed the steps previously explained.

// Implement using pthreads and pthreads mutex locks.

// Your program should print the worker that is moving stones, the stones they are moving, and when the worker finished moving any stone.

// NOTE: Assume that there is at least 2 construction workers.

// NOTE: If there are more than 2 stones to choose from, select stones A-1 and A.

// NOTE: You can use int rand() function from the standard library (stdlib.h) to get a random integer.

// NOTE: Code is compiled with gcc on a posix compliant operating system

// To ease coding on a IDE (VSCode) copy all code from the answer box and paste it on IDE text editor. To compile run build task (Terminal>Run Build Task...). To run the code open a new terminal (Terminal>New Terminal) if there isn't one already open and type ./prog <arg> (substitute <arg> to number of workers to test your code).

// The output should look like this: (remember the order of the lines is not enforced)

// Worker 0 is moving stones 0 and 1
// Worker 0 finished moving 2 stones
// Worker 0 is moving stone 1
// Worker 0 finished moving 1 stone
// Worker 1 is moving stones 0 and 1
// Worker 1 finished moving 2 stones
// Worker 1 is moving stone 0
// Worker 1 finished moving 1 stone
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

pthread_t *tid;
pthread_mutex_t *stones;
bool *finished;

int N;

void *worker(void *arg)
{
    int id = *(int *)arg;
    srand(time(NULL) + id); // seed the random number generator

    int moves = 0;
    int pos = id;
    int stone1 = -1, stone2 = -1;
    bool moved2 = false;
    int sleep_time = rand() % 100;

    while (moves < N)
    {
        // Wait for two available stones
        if (stone1 == -1 || stone2 == -1)
        {
            pthread_mutex_lock(&stones[pos]);
            while (true)
            {
                int left = (pos + N - 1) % N;
                int right = (pos + 1) % N;
                if (!finished[left] && !finished[pos] && left != stone1 && pos != stone1)
                {
                    stone1 = left;
                    stone2 = pos;
                    pthread_mutex_lock(&stones[left]);
                    pthread_mutex_lock(&stones[pos]);
                    break;
                }
                if (!finished[pos] && !finished[right] && pos != stone2 && right != stone2)
                {
                    stone1 = pos;
                    stone2 = right;
                    pthread_mutex_lock(&stones[pos]);
                    pthread_mutex_lock(&stones[right]);
                    break;
                }
                pthread_mutex_unlock(&stones[pos]);
                usleep(sleep_time); // wait for a random amount of time before checking again
                pthread_mutex_lock(&stones[pos]);
            }
        }

        // Move stones
        if (moved2)
        {
            printf("Worker %d finished moving 2 stones\n", id);
        }
        else
        {
            printf("Worker %d finished moving 1 stone\n", id);
        }
        moved2 = !moved2;
        if (moved2)
        {
            usleep(sleep_time); // wait for a random amount of time before moving the next stone
            printf("Worker %d is moving stones %d and %d\n", id, stone1, stone2);
        }
        else
        {
            int stone;
            if (pos == stone1)
            {
                stone = stone2;
            }
            else
            {
                stone = stone1;
            }
            usleep(sleep_time); // wait for a random amount of time before moving the next stone
            printf("Worker %d is moving stone %d\n", id, stone);
        }

        // Release the stones and update state
        if (moved2)
        {
            pthread_mutex_unlock(&stones[stone1]);
            pthread_mutex_unlock(&stones[stone2]);
            finished[pos] = true;
            moves++;
        }
        else
        {
            pthread_mutex_unlock(&stones[stone1]);
            finished[stone2] = false;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./program <number of workers>\n");
        return 1;
    }

    N = atoi(argv[1]);
    tid = (pthread_t *) malloc(N * sizeof(pthread_t));
    stones = malloc(N * sizeof(pthread_mutex_t));
    finished = malloc(N * sizeof(bool));
    for (int i = 0; i < N; i++)
    {
        pthread_mutex_init(&stones[i], NULL);
        finished[i] = false;
    }

    int *ids = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&tid[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_mutex_destroy(&stones[i]);
    }
    free(tid);
    free(stones);
    free(finished);
    free(ids);

    return 0;
}
