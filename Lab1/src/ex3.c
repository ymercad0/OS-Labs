#include "include/ex3.h"

void decode(char* cmd){
    //TODO
}

int main(void){
    // Tests Variables
    char cmd[MAX_CHARS];

    //Test 1
    strcpy(cmd, "WE OUE OUT LNGSAO H RWDN!");
    decode(cmd); // Expected: WE ARE LOW ON DOUGHNUTS !
    memset(cmd,0,MAX_CHARS);

    //Test 2
    strcpy(cmd, "S EFEM FNOCEDROE");
    decode(cmd); // Expected: SEND MORE COFFEE
    memset(cmd,0,MAX_CHARS);

    //Test 3
    strcpy(cmd, "CSERULES");
    decode(cmd); // Expected: INVALID
    memset(cmd,0,MAX_CHARS);

    return 0;
}