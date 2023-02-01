#include <signal.h>

void ignore_SIGSEGV() {
    signal(SIGSEGV, SIG_IGN);
}

int main(){

}