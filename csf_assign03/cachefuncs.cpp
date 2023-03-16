#include "cachefuncs.h"
#include "cachestruct.h"
#include <cstring>
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
    if (!is_power_of_two(num_sets))
    {
        std::cerr << "Number of sets must be a power of 2" << std::endl;
        return 1;
    }
    if (!is_power_of_two(num_blocks))
    {
        std::cerr << "Number of blocks must be a power of 2" << std::endl;
    }
    if (!is_power_of_two(num_bytes))
    {
        std::cerr << "Block size must be a power of 2" << std::endl;
        return 3;
    }
    if (num_bytes <= 4)
    {
        std::cerr << "Byte size in each block must be greater than 4." << std::endl;
        return 4;
    }
    if (!allocation && !write)
    {
        std::cerr << "Write-back and no-write-allocate were both specified" << std::endl;
        return 5;
    }

    return 0;
}

// checks if n is power of 2

bool is_power_of_two(uint32_t n)
{
    if (n == 0)
        return 0;

    while (n != 1)
    {
        if (n % 2 != 0)
            return false;
        n >>= 1;
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
    for (uint32_t j = 0; j < num_sets; j++)
    {
        cache.sets.push_back(set);
    }
    return cache;
}

uint32_t readIndex(uint32_t address, uint32_t indexSize, uint32_t indexOffset)
{
    uint32_t index = 0;
    int curr = indexOffset;

    for (int i = 0; i < (int)indexSize; i++)
    {
        if (address & (1 << curr))
        {
            index |= 1 << curr;
        }
        curr++;
    }

    return index;
}

uint32_t readTag(uint32_t address, uint32_t tagOffset)
{
    uint32_t tag = 0;

    for (int curr = tagOffset; curr < 32; curr++)
    {
        if (address & (1 << curr))
        {
            tag |= 1 << curr;
        }
    }

    return tag;
}

void update_timestamp_LRU(uint32_t index, uint32_t tag, Cache &cache)
{
    std::vector<Block> *blocks = &(cache.sets.at(index).blocks);
    std::map<uint32_t, uint32_t> *tagIndex = &(cache.sets.at(index).tagIndex);

    blocks->at(tagIndex->at(tag)).access_time = cache.sets.at(index).timestamp;
    cache.sets.at(index).timestamp++;
}

void update_timestamp_FIFO(uint32_t index, uint32_t tag, Cache &cache)
{
    std::vector<Block> *blocks = &(cache.sets.at(index).blocks);
    std::map<uint32_t, uint32_t> *tagIndex = &(cache.sets.at(index).tagIndex);

    blocks->at(tagIndex->at(tag)).load_time = cache.sets.at(index).timestamp;
    cache.sets.at(index).timestamp++;
}

std::vector<Block>::iterator find_empty_spot(Set &set)
{
    return set.blocks.begin() + set.tagIndex.size();
}

std::vector<Block>::iterator choose_evicted_block(Set &set, bool lru)
{
    uint32_t minimum = set.timestamp;
    std::vector<Block>::iterator oldest_block;
    std::vector<Block>::iterator iter = set.blocks.begin();

    if (lru)
    {

        while (iter != set.blocks.end())
        {
            if (iter->access_time < minimum)
            {
                minimum = iter->access_time;
                oldest_block = iter;
                iter++;
            }
        }
    }
    else
    {

        while (iter != set.blocks.end())
        {
            if (iter->load_time < minimum)
            {
                minimum = iter->load_time;
                oldest_block = iter;
                iter++;
            }
        }
    }

    return oldest_block;
}

void place_in_cache(uint32_t index, uint32_t tag, bool lru, uint32_t num_bytes, Cache &cache, CacheStats &stats)
{
    std::vector<Block>::iterator iter = find_empty_spot(cache.sets.at(index));

    if (iter == cache.sets.at(index).blocks.end())
    {
        iter = choose_evicted_block(cache.sets.at(index), lru);

        if (iter->dirty)
        {
            stats.total_cycles += ((100 << num_bytes) >> 2);
        }

        cache.sets.at(index).tagIndex.erase(iter->tag);
    }

    cache.sets.at(index).timestamp++;

    // updating block
    iter->tag = tag;
    iter->valid = true;
    iter->dirty = false;

    // updating either access or load timestamp depending on the eviction protocol
    if (lru)
    {
        iter->access_time = cache.sets.at(index).timestamp;
        iter->load_time = 0;
    }
    else
    {
        iter->access_time = 0;
        iter->load_time = cache.sets.at(index).timestamp;
    }

    // updating tags in the map
    cache.sets.at(index).tagIndex[tag] = iter - cache.sets.at(index).blocks.begin();
}

bool hit(uint32_t index, uint32_t tag, Cache &cache)
{
    std::map<uint32_t, uint32_t>::iterator iter = (cache.sets.at(index)).tagIndex.find(tag);
    if (iter != (cache.sets.at(index)).tagIndex.end())
    {
        return true;
    }
    return false;
}

void cache_store(uint32_t index, uint32_t tag, bool allocation, bool write, bool lru, uint32_t num_bytes, Cache &cache, CacheStats &stats)
{
    if (hit(index, tag, cache))
    {
        stats.store_hits++;

        if (lru)
        {
            update_timestamp_LRU(index, tag, cache);
        }
        else
        {
            update_timestamp_FIFO(index, tag, cache);
        }

        if (write)
        {
            stats.total_cycles += ((100 << num_bytes) >> 2);
            stats.total_cycles++;
        }
        else
        {
            stats.total_cycles++;

            uint32_t tag_index = cache.sets.at(index).tagIndex[tag];
            cache.sets.at(index).blocks[tag_index].dirty = true;
        }
    }
    else
    {
        stats.total_cycles += ((100 << num_bytes) >> 2);
        stats.store_misses++;

        if (allocation)
        {
            stats.total_cycles++;
            place_in_cache(index, tag, lru, num_bytes, cache, stats);
        }
    }
}

void cache_load(uint32_t index, uint32_t tag, bool lru, uint32_t num_bytes, Cache &cache, CacheStats &stats)
{
    if (hit(index, tag, cache))
    {
        stats.total_cycles++;
        stats.load_hits++;

        if (lru)
        {
            update_timestamp_LRU(index, tag, cache);
        }
    }
    else
    {
        stats.load_misses++;
        stats.total_cycles += ((100 << num_bytes) >> 2); // (100 cycles * number of bytes) / 4
        place_in_cache(index, tag, lru, num_bytes, cache, stats);
    }
}

void cache_handler(char mem_action, uint32_t address_val, bool allocation, bool write, bool lru, uint32_t num_sets, uint32_t num_bytes, Cache &cache, CacheStats &stats)
{
    uint32_t index = readIndex(address_val, num_sets, num_bytes);
    uint32_t tag = readTag(address_val, num_bytes + num_sets);

    if (mem_action == 's')
    {
        stats.total_stores++;
        cache_store(index, tag, allocation, write, lru, num_bytes, cache, stats);
    }
    else if (mem_action == 'l')
    {
        stats.total_loads++;
        cache_load(index, tag, lru, num_bytes, cache, stats);
    }
}

void printStats(CacheStats &stats)
{
    std::cout << "Total loads: " << stats.total_loads << std::endl;
    std::cout << "Total stores: " << stats.total_stores << std::endl;
    std::cout << "Load hits: " << stats.load_hits << std::endl;
    std::cout << "Load misses: " << stats.load_misses << std::endl;
    std::cout << "Store hits: " << stats.store_hits << std::endl;
    std::cout << "Store misses: " << stats.store_misses << std::endl;
    std::cout << "Total cycles: " << stats.total_cycles << std::endl;
}
