#include <iostream>
#include <string>
#include "cachestruct.h"

#ifndef _CACHE_FUNCS
#define _CACHE_FUNCS

bool isNumber(const std::string s);
uint32_t parseNumber(const char *arg);
int parseInput(const char *arg, const char *trueInput, const char *falseInput);
int handle_error(uint32_t num_sets, uint32_t num_blocks, uint32_t num_bytes, bool allocation, bool write);
bool power_of_two(uint32_t n);
Cache cache_initialize(uint32_t num_sets, uint32_t num_blocks);
void cache_handler(char mem_action, char *address, bool allocation, bool write, bool eviction, uint32_t num_sets, uint32_t num_bytes, Cache &cache, CacheStats &stats);
bool hit(uint32_t index, uint32_t tag, Cache cache);
bool cache_store(uint32_t index, uint32_t tag, bool allocation, bool write, bool eviction, uint32_t num_bytes, Cache &cache, CacheStats &stats);
bool cache_load(uint32_t index, uint32_t tag, bool eviction, uint32_t num_bytes, Cache &cache, CacheStats &stats);
uint32_t readIndex(uint32_t address, uint32_t indexSize, uint32_t indexOffset);
uint32_t readTag(uint32_t address, uint32_t tagOffset);

#else
#endif