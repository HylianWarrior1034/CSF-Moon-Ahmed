#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "cachestruct.h"
#include "cachefuncs.h"

int handle_error(int num_sets, int num_blocks, int num_bytes, char *allocation, char *write);
bool power_of_two(int n);

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        std::cerr << "ERROR: Invalid Command Line Argument";
        return -1;
    }

    uint32_t num_sets, num_blocks, num_bytes;
    bool allocation, write, lru;

    try
    {
        num_sets = parseNumber(argv[1]);                                         // number of sets in the cache (a positive power-of-2)
        num_blocks = parseNumber(argv[2]);                                       // number of blocks in each set (a positive power-of-2)
        num_bytes = parseNumber(argv[3]);                                        // number of bytes in each block (a positive power-of-2, at least 4)
        allocation = parseInput(argv[4], "write-allocate", "no-write-allocate"); // true = "write-allocate" or false = "no-write-allocate"
        write = parseInput(argv[5], "write-through", "write-back");              // true = "write-through" or false = "write-back"
        lru = parseInput(argv[6], "lru", "fifo");                                // true = "lru" (least-recently-used) or false = "fifo" evictions
    }
    catch (std::invalid_argument &ex)
    {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    // error handling
    int error = handle_error(num_sets, num_blocks, num_bytes, allocation, write);

    if (error)
    {
        return error;
    }

    // create any data structures that will be used to track the cache
    Cache main_cache = cache_initialize(num_sets, num_blocks);
    CacheStats stats = {0, 0, 0, 0, 0, 0, 0};

    // main cache simulation loop
    char *curr_line;
    while (std::cin >> curr_line)
    {
        char mem_action = curr_line[0]; // obtaining either 's' or 'l'
        char address[11];               // creating char array to store address of current memory access
        address[10] = '\n';
        // loop to copy memory address
        for (int i = 0; i < 10; i++)
        {
            address[i] = curr_line[i + 2];
        }

        cache_handler(mem_action, address, allocation, write, lru, num_sets, num_bytes, main_cache, stats);
    }

    printStats(stats);
    return 0;
}
