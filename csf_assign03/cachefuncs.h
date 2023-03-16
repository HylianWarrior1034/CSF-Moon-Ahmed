#include <iostream>
#include <string>
#include <math.h>
#include "cachestruct.h"

#ifndef _CACHE_FUNCS
#define _CACHE_FUNCS

bool isNumber(const std::string s);
uint32_t parseNumber(const char *arg);
int parseInput(const char *arg, const char *trueInput, const char *falseInput);
int handle_error(uint32_t num_sets, uint32_t num_blocks, uint32_t num_bytes, bool allocation, bool write);
bool is_power_of_two(uint32_t n);
Cache cache_initialize(uint32_t num_sets, uint32_t num_blocks);
void cache_handler(char mem_action, uint32_t address_val, bool allocation, bool write, bool eviction, uint32_t num_sets, uint32_t num_bytes, Cache &cache, CacheStats &stats);
void update_timestamp_LRU(uint32_t index, uint32_t tag, Cache &cache);
void update_timestamp_FIFO(uint32_t index, uint32_t tag, Cache &cache);
std::vector<Block>::iterator find_empty_spot(Set &set);
std::vector<Block>::iterator choose_evicted_block(Set &set, bool eviction);
void place_in_cache(uint32_t index, uint32_t tag, bool eviction, uint32_t num_bytes, Cache &cache, CacheStats &stats);
bool hit(uint32_t index, uint32_t tag, Cache &cache);
void cache_store(uint32_t index, uint32_t tag, bool allocation, bool write, bool eviction, uint32_t num_bytes, Cache &cache, CacheStats &stats);
void cache_load(uint32_t index, uint32_t tag, bool eviction, uint32_t num_bytes, Cache &cache, CacheStats &stats);
uint32_t log_2(uint32_t num);
uint32_t readIndex(uint32_t address, uint32_t num_sets, uint32_t num_bytes);
uint32_t readTag(uint32_t address, uint32_t num_sets, uint32_t num_bytes);
void printStats(CacheStats &stats);

#else
#endif