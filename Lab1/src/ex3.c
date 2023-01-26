#include "include/ex3.h"

void decode(char *cmd)
{
    size_t length = strlen(cmd);
    if (sqrt(length) != (int) sqrt(length)){
        printf("INVALID\n");
        return;
    }

    length = sqrt(length); // Length of row
    // going through the first row and repeatedly adding len to get column order chars
    for(int i = 0; i < length; i++){
        // Keep track of i to repeatedly increment it
        int new_pos = i;
        for(int j = 0; j < length; j++){
            printf("%c", cmd[new_pos]);
            new_pos += length; // post increment (edge case: i = 0 and new_pos = 0 for first char)
        }
    }
    printf("\n");
}

int main(void)
{
    // Tests Variables
    char cmd[MAX_CHARS];

    // Test 1
    strcpy(cmd, "WE OUE OUT LNGSAO H RWDN!");
    decode(cmd); // Expected: WE ARE LOW ON DOUGHNUTS !
    memset(cmd, 0, MAX_CHARS);

    // // Test 2
    strcpy(cmd, "S EFEM FNOCEDROE");
    decode(cmd); // Expected: SEND MORE COFFEE
    memset(cmd, 0, MAX_CHARS);

    // Test 3
    strcpy(cmd, "CSERULES");
    decode(cmd); // Expected: INVALID
    memset(cmd, 0, MAX_CHARS);

    return 0;
}