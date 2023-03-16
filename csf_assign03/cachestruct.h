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
    std::map<uint32_t, uint32_t> tagIndex;
    uint32_t timestamp;
} Set;

typedef struct
{
    std::vector<Set> sets;
} Cache;

typedef struct
{
    int total_loads;
    int total_stores;
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cycles;
} CacheStats;

#else
#endif