#include "include/main.h"

int lastfit(int low, int startscan, int pages, int memflags, int *len)
{
    int run_length = 0, i;
 	int freerange_start = startscan;
 	int current_last_fit_size = -1;
    int start_last_fit = -1;

    printf("---LAST FIT START---\n");
    printf("Initial position of the search: %d\n", startscan);
    printf("Limit position of the search: %d\n", low);
    printf("Number of pages needed: %d\n", pages);
    
    for (i = low; i <= startscan; i++) {
                printf("page: %d - %d\n",i, page_isfree(i));
            }
    
    for(i = startscan; i >= low; i--) {
 		if(!page_isfree(i)) {
 			int pi;
 			int chunk = i/BITCHUNK_BITS, moved = 0;
 			run_length = 0;
 			pi = i;
 			while(chunk > 0 &&
 			   !MAP_CHUNK(free_pages_bitmap, chunk*BITCHUNK_BITS)) {
 				chunk--;
 				moved = 1;
 			}
 			if(moved) { i = chunk * BITCHUNK_BITS + BITCHUNK_BITS; }
 			continue;
 		}
 		if(!run_length) { freerange_start = i; run_length = 1; }
        else { run_length++; }
 		    
 		if(run_length >= pages && !page_isfree(i-1)) { // This is a Hole Candidate
 			/* good block found! */
 			printf("--HOLE CANDIDATE FOUND--\n");
 			printf("-LATER FIT FOUND-\n");
            
            start_last_fit = freerange_start;
            current_last_fit_size = run_length;

 			printf("Hole Start: %d\n", freerange_start);
            printf("Hole End: %d\n", freerange_start - (run_length - 1));
            printf("Hole Size: %d\n", run_length);
 		}
 	}
 	
 	
    printf("---LAST FIT END---\n");
    if (start_last_fit != -1) {
        printf("-LAST FIT FOUND-\n");
        printf("Hole Start: %d\n", start_last_fit);
        printf("Hole End: %d\n", start_last_fit - (current_last_fit_size - 1));
        printf("Hole Size: %d\n", current_last_fit_size);
        *len = pages;
        return start_last_fit;
    }
    
 	return NO_MEM;
}
