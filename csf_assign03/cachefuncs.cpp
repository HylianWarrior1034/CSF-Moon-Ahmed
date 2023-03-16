#include "cachefuncs.h"
#include "cachestruct.h"
#include <string>

// check if string is a number

bool isNumber(const std::string s)
{
    for (int i = 0; i < (int)s.length(); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// return the char* arg as an int
// if arg is not an integer, throw invalid argument exception

uint32_t parseNumber(const char *arg)
{
    if (!isNumber((std::string)arg))
    {
        throw std::invalid_argument("ERROR: Invalid Command Line Argument");
    }
    return (uint32_t)std::atoi(arg);
}

// return true if arg is equal to trueInput
// return false if arg is equal to falseInput
// throw invalid argument exception if arg is neither

int parseInput(const char *arg, const char *trueInput, const char *falseInput)
{
    if (strcmp(arg, trueInput) == 0)
    {
        return true;
    }
    else if (strcmp(arg, falseInput) == 0)
    {
        return false;
    }
    throw std::invalid_argument("ERROR: Invalid Command Line Argument");
}

// handles error by checking if the criteria are met
// num_sets, num_blocks, and num_bytes all have to be a power of 2.
// num_bytes have to be greater or equal to 4
// no-write-allocate and write-back cannot be used in conjuction

int handle_error(uint32_t num_sets, uint32_t num_blocks, uint32_t num_bytes, bool allocation, bool write)
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
    if (num_bytes <= 4)
    {
        std::cerr << "Byte size in each block must be greater than 4.";
        return 4;
    }
    if (!allocation && !write)
    {
        std::cerr << "Write-back and no-write-allocate were both specified";
        return 5;
    }
}

// checks if n is power of 2

bool power_of_two(uint32_t n)
{
    if (n == 1)
    {
        return true;
    }

    while (n % 2 == 0)
    {
        n >>= 1;
    }

    if (n)
    {
        return false;
    }
    return true;
}

Cache cache_initialize(uint32_t num_sets, uint32_t num_blocks)
{
    Block block = {0, 0, 0, false, false};
    Set set;
    set.timestamp = 0;
    Cache cache;
    for (uint32_t i = 0; i < num_blocks; i++)
    {
        set.blocks.push_back(block);
    }
    for (uint32_t i = 0; i < num_sets; i++)
    {
        cache.sets.push_back(set);
    }
    return cache;
}

uint32_t readIndex(uint32_t address, uint32_t indexSize, uint32_t indexOffset) {
    uint32_t index = 0;
    int curr = indexOffset;
    for (int i = 0; i < indexSize; i++) {
        if (address & (1 << curr)) {
            index |= 1 << curr;
        }
        curr++;
    }
    return index;
}

uint32_t readTag(uint32_t address, uint32_t tagOffset) {
    uint32_t tag = 0;
    for (int curr = tagOffset; curr < 32; curr++) {
        if (address & (1 << curr)) {
            tag |= 1 << curr;
        }
    }
    return tag;
}

void cache_handler(char mem_action, char *address, char *allocation, char *write, char *eviction, int num_sets, int num_bytes, Cache &cache, CacheStats &stats)
{
    uint32_t address_val = std::stoi(address + 2, 0, 16);
    uint32_t index = readIndex(address_val, num_sets, num_bytes);
    uint32_t tag = readTag(address_val, num_bytes + num_sets);
    // TODO: obtain tag and index from the address bits and then either call the load or save function
}

// TODO: define other functions here