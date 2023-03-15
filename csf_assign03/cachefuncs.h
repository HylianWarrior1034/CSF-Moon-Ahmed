#include <iostream>
#include <string.h>
#include "cachestruct.h"

#ifndef _CACHE_FUNCS
#define _CACHE_FUNCS
void cache_handler(char mem_action, char* address, char* allocation, char* write, char* eviction, int num_sets, int num_bytes, Cache &cache, CacheStats &stats);
void cache_save(int index, uint32_t tag, char* eviction, int num_bytes, Cache &cache, CacheStats &stats);
void cache_load(int index, uint32_t tag, char* allocation, char* write, char* eviction, int num_bytes, Cache &cache, CacheStats &stats);
void cache_write_allocate();
void cache_no_write_allocate();
void cache_write_through();
void cache_write_back();
#else
#endif