#include "include/ex4.h"

int main (void)
{

    /*
    Type the following characters on the console (stdin) to test your implementation.
        4
        CCC
        BBB
        DDD
        AAA

    Expected output:
        AAA
        BBB
        CCC
        DDD
    */

    int nlines;			/*number of input lines read */
    scanf("%d\n", &nlines);

    char *word; // String that points to a value. Points to a copy of the current word to get its value
    char **word_list = (char **) malloc(nlines * sizeof(char *)); // Array of strings
    char new_word[MAX_CHARS]; // Keep track of the new words inputted. Holds the value of the current word

    // READ WORDS AND POINT EACH ELEMENT OF WORD_LIST TO A WORD
    for (int i = 0; i < nlines; i++) {
        scanf("%s", new_word);
        word = (char *) malloc(strlen(new_word)-1 * sizeof(char)); // Set word to the length of the new word
        word = strcpy(word, new_word);  // Copies new_word to word and returns the copied string
        word_list[i] = word;
    }

    qsort_((void **) word_list, 0, nlines - 1, (int (*)(void *, void *)) (strcmp));

    // PRINT SORTED ELEMENTS:
    for (int i = 0; i < nlines; i++) {
        printf("%s\n", word_list[i]);
    }

}

void qsort_ (void **v, int left, int right, int (*comp) (void *, void *))
{
    int i, last;
    void swap (void *v[], int, int);

    if (left >= right)
        return;
    swap (v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if ((*comp) (v[i], v[left]) < 0)
            swap (v, ++last, i);
    swap (v, left, last);

   qsort_ (v, left, last - 1, comp);
   qsort_ (v, last + 1, right, comp);
}


void swap (void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

