CONTRIBUTIONS
Daniel Moon (dmoon9)
Ihsan Ahmed (iahmed6)

dmoon9: Discussed code with partner and implemented it together. Debugged hella

iahmed6: Discussed code with partner and implemented it together. Debugged hella

Synchronization Report:

We determined the critical sections to be the the accessing of different sets that could be
accessed by different threads at the same time. If any of these sets were modified by a thread
while being accessed by a different thread, there would be synchronization issues since the
set at the beginning of the "access" would not be guaranteed to be the same. These sets included
the UserSet members in room.cpp and MessageQueue mqueue in message_queue.cpp. When accessing the
member set in room, we added a guard object around the places where we accessed the set (bounded
by curly brackets) to indicate that the set could not be modified during access by any one of the
threads. The methods in which these guard objects were used were add_member, remove_member, and
broadcast_message since these methods involved the modification of members or mqueue sets.
Following a similar note, the methods in which guard objects were used for synchronization in
message_queue.cpp were the destructor, ::enqueue, and ::dequeue since these methods modified
the mqueue set. The guard objects all use mutex to lock and unlock a thread, and semaphore
was used in ::enqueue and ::dequeue to indicate when an element was added to the queue and
when an element was popped from queue. When added, sem_post was used and when popped, sem_timedwait
was used to wait for a sem_post.

We avoid race conditions by allowing access to the sharedsets (mqueue and UserSet) to only one
thread at a time using Guard objects which ensures that only one thread may be accessing the shared
data at a given time. We avoid deadlocks by using Guard objects since Guard objects are guaranteed to
release the locks since it is bounded locally inside a class method, so it is destroyed before the
function returns, meaning that the lock is unlocked before the function is returned since the
destructor unlocks the mutex.
