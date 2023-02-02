/* Example of use of fork system call */
#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed!\n");
        exit(-1);
    }

    // Child succesfully created
    else{
        // Parent will always have the child's PID returned to it
        printf("Creation of child process with the following PID: %d\n", pid);

        if (pid==0) {
            execlp("/bin/ps", "ps", NULL);
            printf("Still around...\n");
            // Get relevant PID info
            int my_pid = getpid(), ppid = getppid();
            printf("I am the child, my process ID is: %d\n", my_pid);
            printf("My parent's PID is: %d\n", ppid);
            exit(0);
        }

        // Switch over to the parent process
        wait(&pid); // Wait for child process to end before exiting
        exit(0); // exit whatever process is currently running
    }

}