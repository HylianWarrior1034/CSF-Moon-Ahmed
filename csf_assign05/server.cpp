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

void chat_with_sender(ConnInfo *info, Message *client_msg)
{
  User *user = new User(msg->data);
  bool exit_case = false;
  Message *server_msg = new Message();
  while (info->client_connection->is_open() && !exit_case)
  {
    if (info->client_connection->receive(*client_msg))
    {

      if (client_msg->tag == TAG_JOIN) // handle join
      {
        info->server->find_or_create_room(client_msg->data);
        user->room_name = client_msg->data;
        server_msg->tag = TAG_OK;
        server_msg->data = "Joined room!";
      }
      else if (sender_msg.tag == TAG_SENDALL)
      {
        if (user->room == "")
        {
          server_msg->tag = TAG_ERR;
          server_msg->data = "Must join room before sending message.";
        }
        else
        {
          Room *room = info->server->find_or_create_room(user->room_name);
          room->broadcast_message(user->username, client_msg->data);
          server_msg->tag = TAG_OK;
          server_msg->data = "Message sent!";
        }
      }
      else if (sender_msg.tag == TAG_LEAVE)
      {
        if (user->room_name == "")
        {
          server_msg->tag = TAG_ERR;
          server_msg->data = "You can't leave a room if you're not in one, ya dingas.";
        }
        else
        {
          user->room_name = "";
          server_msg->tag = TAG_OK;
          server_msg->data = "Left room!";
        }
      }
      else if (sender_msg.tag == TAG_QUIT)
      {
        user->room_name = "";
        server_msg->tag = TAG_OK;
        server_msg->data = "Good bye!";
        exit_case = true;
      }
      else
      {
        server_msg->tag = TAG_ERR;
        server_msg->data = "Invalid message.";
      }

      if (!info->client_connection->send(*server_msg))
      {
        exit_case = true; // break out of loop if sending the message fails
      }
    }
  }
}

void chat_with_receiver(ConnInfo *info, Message *client_msg)
{
  Room *room = nullptr;

  User *user = new User(username);

  Message *server_msg = new Message();

  if (!(info->client_connection->receive(*client_msg) && client_msg->tag == TAG_JOIN && client_msg->data != ""))
  {

    reply->tag = TAG_ERR;
    reply->data = "You must join a room immediately!";
    info->client_connection->send(*reply);
    return;
  }

  room = info->server->find_or_create_room(client_msg->data);
  room->add_member(user);
  server_msg->tag = TAG_OK;
  server_msg->data = "Joined room, welcome to the party!";
  info->client_connection->send(*reply);

  while (1)
  {
    if (!info->client_connection->is_open())
    {
      break;
    }
    Message *broadcast = user->mqueue.dequeue();
    if (broadcast == nullptr)
    {
      continue;
    }

    if (!info->client_connection->send(*broadcast))
    {
      delete broadcast;
      break;
    }
    delete broadcast;
  }

  room->remove_member(receiver);
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

    ConnInfo *info = (ConnInfo *)arg;

    Message *init_msg = new Message();

    if (info->client_connection->receive(*init_msg))
    {

      Message login_confirmation(TAG_OK, "Logged in as " + init_msg->data + "!");

      if (init_msg->tag == TAG_RLOGIN)
      {
        info->client_connection->send(login_confirmation);
        chat_with_receiver(info, init_msg);
      }
      else if (init_msg->tag == TAG_SLOGIN)
      {
        info->client_connection->send(login_confirmation);
        chat_with_sender(info, init_msg);
      }
      else
      {
        Message *server_msg = new Message(TAG_ERR, "First message must be a login request.");
        info->client_connection->send(*server_msg);
      }
    }
    else
    {
      Message *server_msg = new Message(TAG_ERR, "Server did not receive anything from client.");
      info->client_connection->send(server_msg);
    }

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
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server()
{
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen()
{
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string port_str = std::to_string(m_port);
  const char *port = port_str.c_str();
  m_ssock = open_listenfd(port);
  if (m_ssock < 0)
  {
    return false;
  }

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

    struct ConnInfo *info = (ConnInfo *)malloc(sizeof(struct ConnInfo));
    info->clientfd = clientfd;
    info->client_connection = new Connection(clientfd);
    info->server = this;

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
