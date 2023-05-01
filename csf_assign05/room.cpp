#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
    : room_name(room_name)
{
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room()
{
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user)
{
  // TODO: add User to the room
  {
    Guard guard(lock);
    members.insert(user);
    user->room_name = room_name;
  }
}

void Room::remove_member(User *user)
{
  // TODO: remove User from the room
  {
    Guard guard(lock);
    members.erase(user);
    // user has no room now
    user->room_name = "";
  }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text)
{
  {
    Guard guard(lock);
    // TODO: send a message to every (receiver) User in the room
    std::string message = room_name + ":" + sender_username + ":" + message_text;
    UserSet::iterator it;
    for (it = members.begin(); it != members.end(); it++)
    {
      Message *broadcast = new Message(TAG_DELIVERY, message);
      (*it)->mqueue.enqueue(broadcast);
    }
  }
}
