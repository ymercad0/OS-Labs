/* Example of use of fork system call */
#include <stdio.h>    // Prints
#include <unistd.h>   // Forks
#include <stdlib.h>   // Provides declaration for 'exit'
#include <sys/wait.h> // Used for holding processes

int main()
{
    int pid;
    int status;    // wait(int *status_ptr) simply stores status information here
    pid = fork();  // Parent returns result to child and child returns PID to parent

    if (pid < 0) {
        fprintf(stderr, "Fork failed!\n");
        exit(-1);
    }

    // Runs only the child process
    else if (pid==0) {
        // Get relevant PID info
        printf("I am the child, my PID is: %d\n", getpid());
        printf("My parent's PID is: %d\n", getppid());
        // execlp replaces the current process image, meaning anything after won't be executed.
        // It won't return to the calling process.
        printf("Still around...\n");
        execlp("/bin/ps", "ps", NULL);
        exit(0);
    }

    // Runs the parent process
    else{
        printf("I am the parent, the child's PID is: %d\n", pid);
        wait(&status); // Wait for child process to end before exiting
        exit(0); // exit the parent process after the child has ended
    }
}