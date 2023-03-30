Daniel Moon (dmoon9)
Ihsan Ahmed (iahmed6)

Part b (Empirical Evaluation):
For our experiement we mixed and matched conigurations with LRU or FIFO and 
write-allocate and write-back or write-allocate and write-through or 
no-write-allocate and write-through. We also used several different cache size 
configurations, which were (in sets-blocks-bytes_per_block format) 1024-1-4, 
256-4-4, 1-1024-4, 1024-1-256, 256-4-256, and 1-1024-256. Althogether, this was
a total of 36 different configurations being tested. We ran all of these 
configurations with both the gcc.trace and swim.trace files, since these files 
represent traces from real program, making them a good fit to determine 
real-world performance.

After each test, we calculated the load hit rate and the store hit rate to 
gauge the effectiveness of each configuration. Looking at the results, the 
first thing we noticed was that configurations with write-allocate and 
write-through were by far the worst performing configurations. Every one of 
them resulted in total clock cycles that were an order of magnitude greater 
than most of the other results. We also noticed that, with both trace files, 
write-allocate and write-back had the best performance across the board. It 
had the lowest number of clock cycles, best load hit rates, and best store hit 
rates with all cache size configurations (and in both LRU and FIFO configs).
Between LRU and FIFO, the difference between them was sometimes so small that 
it could be negligible, but usually LRU had lower clock cycles and better load 
and store hit rates. In terms of cache size configurations, we found the that 
1 set, 1024 blocks, and 4 bytes per block had the best load/store hit rates and
lowest clock cycles among the configurations with 4096-byte caches. Among the 
configurations with 262144-byte caches, 1 set, 1024 blocks, and 256 bytes per 
block had the best load/store hit rates and lowest clock cycles.

With all of this data, we believe that a configuration with LRU and 
write-allocate and write-back is undoubtedly the best configuration. As for the
cache-size configurations, it would have to be between 1 set, 1024 blocks, and 
4 bytes (1-1024-4) and 1 set, 1024 blocks, and 256 bytes (1-1024-256). From 
purely the hit rates, 1-1024-256 appears to be better in both load hit rate and
store hit rate. However, even though it has lower hit rates (partiularly its 
store hit rate which can be around 20% less) 1-1024-4 still always results in 
fewer clock cycles in the final results, not to mention its cache size is 64 
times smaller than 1-1024-256. Because of this, we have determine that 1-1024-4
is the better cache size configuration. This means that the best overall 
configuration, according to our data, is LRU, write-allocate, and write-back 
with 1 set, 1024 blocks per set, and 4 bytes per block.


Contributions:

dmoon9: Worked on code and discussed implementation with partner and made python auto tester.

iahmed6: Worked on code and discussed implementation with partner and wrote empirical evaluation.
