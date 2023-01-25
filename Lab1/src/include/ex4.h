#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHARS 100
#define MAX_LINES 100		/* max #lines to be sorted */

void qsort_ (void *v[], int left, int right, int (*comp) (void *, void *));