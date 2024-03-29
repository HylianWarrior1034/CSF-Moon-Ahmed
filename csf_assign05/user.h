#ifndef USER_H
#define USER_H

#include <string>
#include "message_queue.h"

struct User
{
  std::string username;
  std::string room_name; // added room_name to keep track of the room the user is in
                         // more easily

  // queue of pending messages awaiting delivery
  MessageQueue mqueue;

  User(const std::string &username) : username(username), room_name("") {}
};

#endif // USER_H
