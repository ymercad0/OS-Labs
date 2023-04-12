int bestfit(int low, int startscan, int pages, int memflags, int *len)
{
    int run_length = 0, i;
	int freerange_start = low;
    int pSize = startscan - low;
    char found = 0; // for conditionals
	int RETURN = NO_MEM;
    int curr_freerange_start;

    printf("---BEST FIT START---\n");
	printf("Initial position of the search: %d\n", startscan);
	printf("Limit position of the search: %d\n", low);
	printf("Number of pages needed: %d\n", pages);
	for(i = low; i < startscan; i++) {
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
		else { freerange_start++; run_length++; }
		assert(run_length <= pages);
		/* good block found! */
		if(run_length == pages) {
            int newSize = 0;
            found = 1;
            printf("--HOLE CANDIDATE FOUND--\n");
            while(i < startscan) {
                i++;
                if(page_isfree(i)) newSize++;
                else break;
            }

            if (newSize + run_length < pSize){
                printf("-BETTER FIT FOUND-\n");
                pSize = newSize + run_length;
                curr_freerange_start = freerange_start - run_length + 1;
            }
            printf("Hole Start: %d\n", freerange_start - run_length + 1);
            printf("Hole End: %d\n", freerange_start + newSize);
            printf("Hole Size: %d\n", newSize + run_length);
		}
	}
    if(found){
        printf("---BEST FIT END---\n");
        printf("-BEST FIT FOUND-\n");
        printf("Hole Start: %d\n", curr_freerange_start);
        printf("Hole End: %d\n", curr_freerange_start + pSize - 1);
        printf("Hole Size: %d\n", pSize);
        *len = run_length;
		RETURN = freerange_start;
    }
	return RETURN;
}