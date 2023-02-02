#include <signal.h>
#include <stdio.h>

void sig_sigint(){   // Interrupt from keyboard
    printf("Received signal SIGINT!!!");
}

void sig_sigquit(){  // Quit from keyboard
    printf("Received signal SIGQUIT!!!");
}

void sig_sigterm(){  // Termination signal
    printf("Received signal SIGTERM!!!");
}

void sig_sigfpe(){  // Erroneous arithmetic operations
    printf("Received signal SIGFPE!!!");
}

void sig_sigsegv(){  // Invalid virtual memory reference or segfault
    printf("Received signal SIGSEGV!!!");
}

void sig_sigill(){  // Illegal instruction
    printf("Received signal SIGILL!!!");
}

// signal() sets a function to handle a specified signal (int)
void setup_signal_handlers() {
    signal(SIGINT, sig_sigint);
    signal(SIGQUIT, sig_sigquit);
    signal(SIGTERM, sig_sigterm);
    signal(SIGFPE, sig_sigfpe);
    signal(SIGSEGV, sig_sigsegv);
    signal(SIGILL, sig_sigill);
}

int main(){
}