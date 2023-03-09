#include <stdlib.h>
#include <iostream>
#include <cmath.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int num_sets = atoi(argv[1]);   // number of sets in the cache (a positive power-of-2)
    int num_blocks = atoi(argv[2]); // number of blocks in each set (a positive power-of-2)
    int num_bytes = atoi(argv[3]);  // number of bytes in each block (a positive power-of-2, at least 4)
    char *allocation = argv[4];     // "write-allocate" or "no-write-allocate"
    char *write = argv[5];          // "write-through" or "write-back"
    char *eviction = argv[6];       // "lru" (least-recently-used) or "fifo" evictions

    // error handling
    int error = handle_error(num_sets, num_blocks, num_bytes, allocation, write);

    if (error)
    {
        return error;
    }

    // create any data structures that will be used to track the cache

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
    }

    return 0;
}

int handle_error(int num_sets, int num_blocks, int num_bytes, char *allocation, char *write)
{
    if (!power_of_two(num_sets))
    {
        std::cerr << "Number of sets must be a power of 2";
        return 1;
    }
    if (!power_of_two(num_blocks))
    {
        std::cerr << "Number of blocks must be a power of 2";
    }
    if (!power_of_two(num_bytes))
    {
        std::cerr << "Block size must be a power of 2";
        return 3;
    }
    if (num_bytes < 4)
    {
        std::cerr << "Block size in each block must be greater than 4.";
        return 4;
    }
    if (strcmp(allocation, "no-write-allocate") == 0 && strcmp(write, "write-back") == 0)
    {
        std::cerr << "Write-back and no-write-allocate were both specified";
        return 5;
    }
}

bool power_of_two(int a)
{
    while (n % 2 == 0)
    {
        n /= 2;
    }
    if (n)
    {
        return false;
    }
    return true;
}