#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

void chat_with_sender(ConnInfo *info, Message *client_msg, User *user)
{
  // start main loop handling sender requests
  bool exit_case = false;
  Message *server_msg = new Message();
  while (info->client_connection->is_open() && !exit_case)
  {
    // receive sender request
    if (info->client_connection->receive(*client_msg))
    {
      // handle message depending on tag
      if (client_msg->tag == TAG_JOIN) // handle join
      {
        std::string room_name = client_msg->data; // parse room name
        room_name = room_name.substr(0, room_name.size() - 1);
        info->server->find_or_create_room(room_name); // obtain room
        user->room_name = room_name; // set user's room
        // set ok message
        server_msg->tag = TAG_OK;
        server_msg->data = "Joined room!";
      }
      else if (client_msg->tag == TAG_SENDALL)
      {
        // parse message text
        std::string broadcast = client_msg->data;
        broadcast = broadcast.substr(0, broadcast.size() - 1);
        if (user->room_name == "")
        {
          // send error if room name is invalid
          server_msg->tag = TAG_ERR;
          server_msg->data = "Must join room before sending message.";
        }
        else
        {
          Room *room = info->server->find_or_create_room(user->room_name); // obtain room
          room->broadcast_message(user->username, broadcast); // queue message for broadcasting
          // set ok message
          server_msg->tag = TAG_OK;
          server_msg->data = "Message sent!";
        }
      }
      else if (client_msg->tag == TAG_LEAVE)
      {
        if (user->room_name == "")
        {
          // send error if not in a room
          server_msg->tag = TAG_ERR;
          server_msg->data = "You can't leave a room if you're not in one, ya dingas.";
        }
        else
        {
          // exit current room
          user->room_name = "";
          // set ok message
          server_msg->tag = TAG_OK;
          server_msg->data = "Left room!";
        }
      }
      else if (client_msg->tag == TAG_QUIT)
      {
        user->room_name = "";
        // set ok message
        server_msg->tag = TAG_OK;
        server_msg->data = "Good bye!";
        exit_case = true; // exit loop
      }
      else
      {
        // message was invalid; respond with error
        server_msg->tag = TAG_ERR;
        server_msg->data = "Invalid message.";
      }

      if (!info->client_connection->send(*server_msg)) // send response
      {
        exit_case = true; // break out of loop if sending the message fails
      }
    }
  }
}

void chat_with_receiver(ConnInfo *info, Message *client_msg, User *user)
{
  Room *room = nullptr;
  Message *server_msg = new Message();

  // receive message from receiver and respond accordingly
  if (!(info->client_connection->receive(*client_msg) && client_msg->tag == TAG_JOIN && client_msg->data != ""))
  {
    // send error if join message is not received
    server_msg->tag = TAG_ERR;
    server_msg->data = "You must join a room immediately!";
    info->client_connection->send(*server_msg);
    return;
  }

  // parse room name
  std::string room_name = client_msg->data;
  room_name = room_name.substr(0, room_name.size() - 1);
  // obtain room
  room = info->server->find_or_create_room(room_name);
  // add user to room
  room->add_member(user);
  user->room_name = room_name;
  // send ok reply
  server_msg->tag = TAG_OK;
  server_msg->data = "Joined room welcome to the party";
  info->client_connection->send(*server_msg);

  // start main loop broadcasting messages to receiver
  while (1)
  {
    // exit loop if connection closes
    if (!info->client_connection->is_open())
    {
      break;
    }
    // attempt to obtain next message to broadcast to recevier
    Message *broadcast = user->mqueue.dequeue();
    if (broadcast == nullptr)
    {
      continue; // continue loop if there are no messages in queue
    }

    // broadcast message to receiver
    if (!info->client_connection->send(*broadcast))
    {
      // break out of while loop if the send fails
      delete broadcast;
      break;
    }
    delete broadcast;
  }

  // remove receiver from the room when loop terminates
  room->remove_member(user);
}

namespace
{

  void *worker(void *arg)
  {
    pthread_detach(pthread_self());

    // TODO: use a static cast to convert arg from a void* to
    //       whatever pointer type describes the object(s) needed
    //       to communicate with a client (sender or receiver)

    // TODO: read login message (should be tagged either with
    //       TAG_SLOGIN or TAG_RLOGIN), send response

    // TODO: depending on whether the client logged in as a sender or
    //       receiver, communicate with the client (implementing
    //       separate helper functions for each of these possibilities
    //       is a good idea)

    // obtain data structure containing connection with client and server info
    ConnInfo *info = (ConnInfo *)arg;

    Message *init_msg = new Message();
    // receive message from client
    if (info->client_connection->receive(*init_msg))
    {
      // parse message data and create user object
      std::string name = init_msg->data.substr(0, init_msg->data.size() - 1);
      User *new_user = new User(name);
      // create login confirmation message
      Message login_confirmation(TAG_OK, "Logged in as " + name);

      // handle message depending on the tag
      if (init_msg->tag == TAG_RLOGIN)
      {
        // send login confirmation and initiate receiver handling loop
        info->client_connection->send(login_confirmation);
        chat_with_receiver(info, init_msg, new_user);
      }
      else if (init_msg->tag == TAG_SLOGIN)
      {
        // send login confirmation and initiate sender handling loop
        info->client_connection->send(login_confirmation);
        chat_with_sender(info, init_msg, new_user);
      }
      else
      {
        // send error to client if message is not a login message
        Message *server_msg = new Message(TAG_ERR, "First message must be a login request.");
        info->client_connection->send(*server_msg);
      }
    }
    else
    {
      // send error to client if server failed to receive a message from them
      Message *server_msg = new Message(TAG_ERR, "Server did not receive anything from client.");
      info->client_connection->send(*server_msg);
    }

    // destroy appropriate data and exit
    close(info->clientfd);
    free(info);
    return nullptr;
  }

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
    : m_port(port), m_ssock(-1)
{
  // initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server()
{
  // destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen()
{
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string port_str = std::to_string(m_port);
  const char *port = port_str.c_str();
  m_ssock = open_listenfd(port); // open socket
  if (m_ssock < 0)
  {
    return false; // socket could not open
  }
  // socket successfully opened
  return true;
}

void Server::handle_client_requests()
{
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while (1)
  {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0)
    {
      std::cerr << "Error accepting client connection." << std::endl;
    }

    // open connection with client
    struct ConnInfo *info = (ConnInfo *)malloc(sizeof(struct ConnInfo));
    info->clientfd = clientfd;
    info->client_connection = new Connection(clientfd);
    info->server = this;

    // start new thread to handle client
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, worker, info) != 0)
    {
      std::cerr << "Error: Cannot create pthread for client" << std::endl;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name)
{
  Guard guard(m_lock);
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  auto map_iter = m_rooms.find(room_name);
  Room *target_room;

  // existing room is found
  if (map_iter != m_rooms.end())
  {
    target_room = map_iter->second;
    return target_room;
  }

  // no existing room is found
  Room *new_room = new Room(room_name);
  m_rooms.insert({room_name, new_room});

  return new_room;
}
