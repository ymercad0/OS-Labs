#include <signal.h>

void generate_SIGSEGV() {
    raise(SIGSEGV);
}

void generate_SIGFPE() {
    raise(SIGFPE);
}

int main(){

}