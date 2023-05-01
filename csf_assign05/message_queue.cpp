#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"

MessageQueue::MessageQueue()
{
  // TODO: initialize the mutex and the semaphore
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, 0);
}

MessageQueue::~MessageQueue()
{
  // TODO: destroy the mutex and the semaphore
  {
    Guard guard(m_lock);
    std::deque<Message *>::iterator msg_iterator;
    for (msg_iterator = m_messages.begin(); msg_iterator != m_messages.end(); ++msg_iterator)
    {
      delete *msg_iterator;
    }
  }

  sem_destroy(&m_avail);
  pthread_mutex_destroy(&m_lock);
}

void MessageQueue::enqueue(Message *msg)
{
  // curly brackets important because it defines scope of guard
  {
    // TODO: put the specified message on the queue
    Guard guard(m_lock);
    m_messages.push_back(msg);
  }

  sem_post(&m_avail);
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
}

Message *MessageQueue::dequeue()
{

  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  // TODO: call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available
  if (sem_timedwait(&m_avail, &ts) == -1)
  {
    return nullptr;
  }

  Message *msg;

  {
    Guard guard(m_lock);
    // TODO: remove the next message from the queue, return it
    msg = m_messages.front();
    m_messages.pop_front();
  }
  return msg;
}
