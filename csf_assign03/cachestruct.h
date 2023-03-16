#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

#ifndef _CACHE_STRUCT
#define _CACHE_STRUCT
typedef struct
{
    uint32_t tag;
    uint32_t load_time;
    uint32_t access_time;
    bool valid;
    bool dirty;
} Block;

typedef struct
{
    std::vector<Block> blocks;
    std::map<uint32_t, uint32_t> tagIndex; // which index in the blocks vector has the corresponding tag data
    uint32_t timestamp;
} Set;

typedef struct
{
    std::vector<Set> sets;
} Cache;

typedef struct
{
    uint32_t total_loads;
    uint32_t total_stores;
    uint32_t load_hits;
    uint32_t load_misses;
    uint32_t store_hits;
    uint32_t store_misses;
    uint32_t total_cycles;
} CacheStats;

#else
#endif