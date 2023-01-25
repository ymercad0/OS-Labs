#include "include/ex1.h"

void update(int *a, int *b)
{
    int temp_a = *a;
    *a = (*a) - (*b);
    *b = ((*a) % 2 == 0) ? (temp_a) * (*b) : abs((*b) - (temp_a));
}

int main(void)
{
    // Tests Variables
    int a, b;
    int *pa = &a, *pb = &b;

    printf("Exercise 1 tester.\n");

    // Test 1
    *pa = 0;
    *pb = 4;
    update(&a, &b);
    printf("Expected: a = -4, b = 0\t Got: a = %d, b = %d\n", a, b);

    // Test 2
    *pa = 1;
    *pb = 2;
    update(&a, &b);
    printf("Expected: a = -1, b = 1\t Got: a = %d, b = %d\n", a, b);

    // Test 3
    *pa = 10;
    *pb = 4;
    update(&a, &b);
    printf("Expected: a = 6, b = 40\t Got: a = %d, b = %d\n", a, b);

    // Test 4
    *pa = 13;
    *pb = 11;
    update(&a, &b);
    printf("Expected: a = 2, b = 143\t Got: a = %d, b = %d\n", a, b);

    return 0;
}