#include <signal.h>

void generate_SIGSEGV() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    arr[12] = arr[5];
}

void generate_SIGFPE() {
    int a=1, b=0;
    a = a/b;
}

int main(){

}