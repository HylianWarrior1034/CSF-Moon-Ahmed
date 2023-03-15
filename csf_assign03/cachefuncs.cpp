#include "cachefuncs.h"

void cache_handler(char mem_action, char* address, char* allocation, char* write, char* eviction, int num_sets, int num_bytes, Cache &cache, CacheStats &stats){
    uint32_t address_val = std::stoi(address + 2, 0, 16);
    // TODO: obtain tag and index from the address bits and then either call the load or save function
}

// TODO: define other functions here