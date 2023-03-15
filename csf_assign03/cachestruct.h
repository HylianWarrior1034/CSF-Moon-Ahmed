#include <stdlib.h>
#include <iostream>
#include <vector>

#ifndef _CACHE_STRUCT
#define _CACHE_STRUCT
struct Block {
    uint32_t tag;
    bool valid;
    bool dirty;
    uint32_t load_order;
    uint32_t access_order;

    Block();
};

struct Set {
    std::vector<Block> blocks;

    Set();
    Set(int num_blocks);
};

struct Cache {
    std::vector<Set> sets;

    Cache();
    Cache(int num_sets, int num_blocks);
};

struct CacheStats {
    int total_loads;
    int total_stores;
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cycles;

    CacheStats();
};
#else
#endif