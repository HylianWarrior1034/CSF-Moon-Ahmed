#include "cachefuncs.h"
#include <string>

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
int parseNumber(const char *arg)
{
    if (!isNumber((std::string)arg))
    {
        throw std::invalid_argument("ERROR: Invalid Command Line Argument");
    }
    return std::atoi(arg);
}

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

int handle_error(int num_sets, int num_blocks, int num_bytes, bool allocation, bool write)
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
    if (!allocation && !write)
    {
        std::cerr << "Write-back and no-write-allocate were both specified";
        return 5;
    }
}

bool power_of_two(int n)
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

void cache_handler(char mem_action, char *address, char *allocation, char *write, char *eviction, int num_sets, int num_bytes, Cache &cache, CacheStats &stats)
{
    uint32_t address_val = std::stoi(address + 2, 0, 16);
    // TODO: obtain tag and index from the address bits and then either call the load or save function
}

// TODO: define other functions here