CONTRIBUTIONS
Daniel Moon (dmoon9)
Ihsan Ahmed (iahmed6)

dmoon9: Discussed code with partner and implemented it together.

iahmed6: Discussed code with partner and implemented it together.

REPORT

At a threshold of 2097152 our program ran for 0m0.212s, a threshold of 1048576
ran for 0m0.139s, a threshold of 524288 ran for 0m0.127s, a threshold of 262144
ran for 0m0.126s, a threshold of 131072 ran for 0m0.135s, a threshold of 65536 
ran for 0m0.138s, a threshold of 32768 ran for 0m0.138s, and a threshold of 
16384 ran for 0m0.144s.

We can see that (up to a certain point) decreasing the threshold decreases the 
total amount of time that the program takes. This is because the threshold 
parameter defines the maximum number of elements that will be sorted at once in
a qsort. In other words, the smaller the threshold is, the more forks the 
program will create to divide the sorting of the data. More forks means that 
the merge sort is split among more processes that can be parallelized, thus 
saving time in the computation. Of course, the CPU doesn't have an unlimited 
numebr of cores to run each process, so the benefit of decreasing the threshold
begins to dimish after the threshold becomes snmaller than 262144.
