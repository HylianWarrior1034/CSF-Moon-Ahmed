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

void chat_with_sender(Connection *sender_conn, User *sender, Server *server) {
  Room *room;
  while(1)
  {
    Message sender_msg;
    sender_conn->receive(sender_msg);

    Message reply;
    if (sender_msg.tag == TAG_JOIN) {
      room = server->find_or_create_room(sender_msg.data);
      room->add_member(sender);
      reply.tag = TAG_OK;
      reply.data = "Joined room!";
    } else if (sender_msg.tag == TAG_SENDALL) {
      room->broadcast_message(sender->username, sender_msg.data);
      // figure out how to send message
      reply.tag = TAG_OK;
      reply.data = "Message sent!";
    } else if (sender_msg.tag == TAG_LEAVE) {
      room->remove_member(sender);
      reply.tag = TAG_OK;
      reply.data = "Left room!";
    } else if (sender_msg.tag == TAG_QUIT) {
      reply.tag = TAG_OK;
      reply.data = "Good bye!";
    } else {
      // error
    }

    sender_conn->send(reply);

  }
}

void chat_with_receiver(Connection *receiver_conn, User *receiver, Server *server) {
  Room *room = nullptr;

  Message receiver_msg;
  receiver_conn->receive(receiver_msg);

  Message reply;
  if (receiver_msg.tag == TAG_JOIN) {
    room = server->find_or_create_room(receiver_msg.data);
    room->add_member(receiver);
    reply.tag = TAG_OK;
    reply.data = "Joined room!";
    receiver_conn->send(reply);
  } else {
    // error
  }

  while(1)
  {
    Message *broadcast = receiver->mqueue.dequeue();
    if (broadcast != nullptr) {
      receiver_conn->send(*broadcast);
    }
    
    // figure out how to determine when the receiver has closed their programs and close the data structures accordingly (has something to do with SIGPIPE signal)

  }
}

namespace {

void *worker(void *arg) {
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

  ConnInfo *info = (ConnInfo *) arg;
  struct Connection *client_connection = info->client_connection;
  Server *server = info->server;

  Message init_msg;
  client_connection->receive(init_msg);
  
  User *new_user = &User(init_msg.data);
  Message login_confirmation(TAG_OK, "Logged in!");

  if (init_msg.tag == TAG_RLOGIN) {
    client_connection->send(login_confirmation);
    chat_with_receiver(client_connection, new_user, server);
  } else if (init_msg.tag == TAG_SLOGIN) {
    client_connection->send(login_confirmation);
    chat_with_sender(client_connection, new_user, server);
  } else {
    // error
  }

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  int serverfd = open_listenfd((char *) m_port);
  if (serverfd < 0)
  {
    std::cerr << "Unable to open server socket." << std::endl;
    return false;
  }

  m_ssock = serverfd;
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  if(!listen())
  {
    exit(1);
  }

  while (1)
  {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0)
    {
      std::cerr << "Error accepting client connection." << std::endl;
      exit(2);
    }

    struct Connection *client_connection = &Connection(clientfd);
    ConnInfo *info;
    info->client_connection = client_connection;
    info->server = this;

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, worker, info) != 0)
    {
      // error
    }

    // finish while loop later after i figure out room functionality

  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  auto map_iter = m_rooms.find(room_name);
  Room *target_room;
  if (map_iter != m_rooms.end())
  {
    target_room = map_iter->second();
  } else {
    *target_room = Room(room_name);
    m_rooms.insert(std::pair<std::string, Room *>(room_name, target_room));
    target_room = (m_rooms.find(room_name))->second();
  }

  return target_room;
}
