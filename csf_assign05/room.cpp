#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
    : room_name(room_name)
{
  // initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room()
{
  // destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user)
{
  // add User to the room, but need guard since multiple threads can access
  // members at the same time
  {
    Guard guard(lock);
    members.insert(user);
    user->room_name = room_name;
  }
}

void Room::remove_member(User *user)
{
  // remove User from the room with guard (synchronization)
  {
    Guard guard(lock);
    members.erase(user);
    // user has no room now
    user->room_name = "";
  }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text)
{
  // need guard since putting into mqueue may cause conflicts
  {
    Guard guard(lock);
    // send a message to every (receiver) User in the room
    std::string message = room_name + ":" + sender_username + ":" + message_text;
    UserSet::iterator it;

    // iterate through all users (receivers) in the room and send
    for (it = members.begin(); it != members.end(); it++)
    {
      Message *broadcast = new Message(TAG_DELIVERY, message);
      (*it)->mqueue.enqueue(broadcast);
    }
  }
}
