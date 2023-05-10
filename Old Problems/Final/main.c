#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

int threads;
double sol[100][100];

typedef struct matrix{
    int data[100][100];
    int size1;
    int size2;
} matrix;

typedef struct ThreadInput {
    int start;
    int end;
    int scalar;
    matrix* A;
    matrix* B;
} ThreadInput;

void printSol(int size1, int size2) {
    printf("Result =\n[");
    for (int row = 0; row < size1; row ++) {
        printf("[");
        for(int col = 0; col < size2; col++) {
            printf("%f", sol[row][col]);
            if (col != size2-1) {
                printf(", ");
            }
        }

        if (row != size1-1) {
            printf("],\n");
        }else {
            printf("]");
        }
        
    }
    printf("]\n");
}

void *runner(void *param){
    ThreadInput* input = (ThreadInput*) param;

    for (int i = input->start; i < input->end && i < input->A->size1; i++) {
        for (int j = 0; j < input->A->size2; j++) {
            sol[i][j] = ((double)(input->A->data[i][j] - input->B->data[i][j]))/((double)input->scalar);
        }
    }

    return NULL;
}

void matrixOperation(int scalar, struct matrix X, struct matrix Y){
    //TODO
    if (X.size1 != Y.size1 || X.size2 != Y.size2) {
        printf("ILLEGAL OPERATION\n");
        return;
    }
    
    pthread_t workers[threads];
    ThreadInput inputs[threads];

    int objectsPerThread = ceil(((double)(X.size1))/((double)threads));
    int count = 0;

    for (int i = 0; i < threads; i++) {
        inputs[i] = (ThreadInput) {
            .start = count,
            .end = count + objectsPerThread,
            .scalar = scalar,
            .A = &X,
            .B = &Y
        };
        count += objectsPerThread;
        pthread_create(&workers[i], NULL, runner, (void *) &inputs[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(workers[i], NULL);
    }

    printSol(X.size1, Y.size2);

    return;
}

/**
 Result =
[[-1.333333, -1.333333],
[-1.333333, -1.333333]]
Result =
[[-0.500000, -2.000000],
[-4.500000, 1.500000]]
ILLEGAL OPERATION
 */

int main(){
    struct matrix X, Y;
    X.data[0][0] = 1;
    X.data[0][1] = 2;
    X.data[1][0] = 3;
    X.data[1][1] = 4;
    X.size1 = 2;
    X.size2 = 2;
    Y.data[0][0] = 5;
    Y.data[0][1] = 6;
    Y.data[1][0] = 7;
    Y.data[1][1] = 8;
    Y.size1 = 2;
    Y.size2 = 2;
    threads = 1;
    matrixOperation(3,X,Y);
    threads = 2;
    Y.data[1][0] = 12;
    Y.data[1][1] = 1;
    Y.data[0][0] = 2;
    matrixOperation(2,X,Y);
    Y.size2 = 4;
    threads = 2;
    matrixOperation(2,X,Y);
}