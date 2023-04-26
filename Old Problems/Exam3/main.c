#include "include/main.h"
void assert(int a){}
int main() {
   {
       int *len;
       int i;
        
        for(i=0;i<=3;i++){
            free_pages_bitmap[i]=1;
        }
        for(i=4;i<=9;i++){
            free_pages_bitmap[i]=0;
        }
        for(i=10;i<=15;i++){
            free_pages_bitmap[i]=1;
        }
        for(i=16;i<=20;i++){
            free_pages_bitmap[i]=0;
        }
        for(i=21;i<=25;i++){
            free_pages_bitmap[i]=1;
        }
        lastfit( 1, 25, 3, 1, (int*)&len);
    }
    return 0;
}