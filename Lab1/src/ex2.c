#include "include/ex2.h"

// n, amount of series to compute
void tribonacci(int n)
{
    // initialize array (int* seq) to 1, using malloc/calloc
    int *seq = (int *)malloc(1 * sizeof(int)); // without (int*) malloc returns void pointer. Must cast to the type we need

    for (int i = 0; i <= n; i++)
    {

        // Recompute the whole series
        tribonacciHelper(i, seq);

        // print 1D array
        for (int j = 0; j <= i; j++)
        {
            printf("%d ", seq[j]);
        }
        printf("\n");
        // resize array, with realloc (else we end up with extra empty spaces)
        seq = (int *)realloc(seq, (i + 2) * sizeof(int));
    }
    // free array
    free(seq);
}

// n=amount of numbers in the series to compute, seq=array to store series
void tribonacciHelper(int n, int *seq)
{ // fills up one pass/row
    if (n == 0)
    {
        seq[0] = 0; // Hard code first 3 values
    }

    else if (n == 1)
    {
        seq[1] = 1;
    }

    else if (n == 2)
    {
        seq[2] = 1;
    }
    else
    {
        seq[n] = seq[n - 1] + seq[n - 2] + seq[n - 3]; // only fill up those extra values
    }
}

int main(void)
{
    /*
    Reference Expected Value (from tribonacci(20))
    0
    0 1
    0 1 1
    0 1 1 2
    0 1 1 2 4
    0 1 1 2 4 7
    0 1 1 2 4 7 13
    0 1 1 2 4 7 13 24
    0 1 1 2 4 7 13 24 44
    0 1 1 2 4 7 13 24 44 81
    0 1 1 2 4 7 13 24 44 81 149
    0 1 1 2 4 7 13 24 44 81 149 274
    0 1 1 2 4 7 13 24 44 81 149 274 504
    0 1 1 2 4 7 13 24 44 81 149 274 504 927
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136 5768
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136 5768 10609
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136 5768 10609 19513
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136 5768 10609 19513 35890
    0 1 1 2 4 7 13 24 44 81 149 274 504 927 1705 3136 5768 10609 19513 35890 66012
    */

    printf("Exercise 2 tester.\n");

    // Test 1
    printf("Test 1\n");
    tribonacci(3);
    // Test 2
    printf("\nTest 2\n");
    tribonacci(0);
    // Test 3
    printf("\nTest 3\n");
    tribonacci(2);
    // Test 4
    printf("\nTest 4\n");
    tribonacci(20);

    return 0;
}