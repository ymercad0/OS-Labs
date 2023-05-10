#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){

    if(argc != 2) fprintf(stderr, "Did not provide input\n");
    if(argv[1] <= 0) fprintf(stderr, "Argument %d must be positive", atoi(argv[1]));

    int n = atoi(argv[1]);
    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if(pid < 0) {
            fprintf(stderr, "Fork Failed\n");
        } else if(pid == 0){
            fork();
            printf("Child PID: %d\n", getppid());
        } else {
            continue;
        }
    }

    return 0;
}