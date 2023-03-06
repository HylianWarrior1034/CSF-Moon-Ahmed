#include <stdlib.h>
#include <iostream>

int main (int argc, char *argv[]) {
    int num_sets = atoi(argv[1]); // number of sets in the cache (a positive power-of-2)
    int num_blocks = atoi(argv[2]); // number of blocks in each set (a positive power-of-2)
    int num_bytes = atoi(argv[3]); // number of bytes in each block (a positive power-of-2, at least 4)
    char *allocation = argv[4]; // "write-allocate" or "no-write-allocate"
    char *write = argv[5]; // "write-through" or "write-back"
    char *eviction = argv[6]; // "lru" (least-recently-used) or "fifo" evictions

    // create any data structures that will be used to track the cache

    // main cache simulation loop
    char *curr_line;
    while (std::cin >> curr_line) {
        char mem_action = curr_line[0]; // obtaining either 's' or 'l'
        char address[11]; // creating char array to store address of current memory access
        address[10] = '\n';
        // loop to copy memory address
        for (int i = 0; i < 10; i++) {
            address[i] = curr_line[i + 2];
        }
    }

    return 0;
}