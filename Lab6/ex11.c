static int findbit(int low, int startscan, int pages, int memflags, int *len)
{
	int run_length = 0, i;
	int freerange_start = startscan;
	int busy_pages = 0;
	int RETURN = NO_MEM;

	printf("Initial position of the search: %d\n", startscan);
	printf("Limit position of the search: %d\n", low);
	printf("Number of pages needed: %d\n", pages);
	for(i = startscan; i >= low; i--) {
		if(!page_isfree(i)) {
			int pi;
			int chunk = i/BITCHUNK_BITS, moved = 0;
			run_length = 0;
			pi = i;
			busy_pages += 1;
			while(chunk > 0 &&
			   !MAP_CHUNK(free_pages_bitmap, chunk*BITCHUNK_BITS)) {
				chunk--;
				moved = 1;
			}
			if(moved) { i = chunk * BITCHUNK_BITS + BITCHUNK_BITS; }
			continue;
		}
		if(!run_length) { freerange_start = i; run_length = 1; }
		else { freerange_start--; run_length++; }
		assert(run_length <= pages);
		if(run_length == pages) {
			/* good block found! */
			*len = run_length;
			RETURN = freerange_start;
			break;
		}
	}
    printf("Position found: %d\n", freerange_start);
	printf("Restarts done before finding contiguous pages: %d\n", busy_pages);
	// Print bitmap bits
	printf("Bitmap: ");
	for (int j = freerange_start; j <= freerange_start + pages; j++){
	    // Flip bits
        printf("%d", 1 ? free_pages_bitmap[j] == 0 : 0);
    }
	return RETURN;
}