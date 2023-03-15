#include "cachestruct.h"

Block::Block() {
    tag = 0;
    valid = 0;
    dirty = 0;
    load_order = 0;
    load_order = 0;
}

Set::Set(int num_blocks) {
    blocks = std::vector<Block>(num_blocks, Block());
}

Cache::Cache(int num_sets, int num_blocks) {
    sets = std::vector<Set>(num_sets, Set(num_blocks));
}

CacheStats::CacheStats() {
    total_loads = 0;
    total_stores = 0;
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_misses = 0;
    total_cycles = 0;
}