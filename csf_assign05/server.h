#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>
#include "user.h"
class Room;
class Server;

struct ConnInfo
{
  int clientfd;
  struct Connection *client_connection;
  Server *server;
};

class Server
{
public:
  Server(int port);
  ~Server();

  bool listen();

  void handle_client_requests();
  void chat_with_sender(Connection *sender_conn, Message *client_msg);
  void chat_with_receiver(Connection *receiver_conn, Message *client_msg);

  Room *find_or_create_room(const std::string &room_name);

private:
  // prohibit value semantics
  Server(const Server &);
  Server &operator=(const Server &);

  typedef std::map<std::string, Room *> RoomMap;

  // These member variables are sufficient for implementing
  // the server operations
  int m_port;
  int m_ssock;
  RoomMap m_rooms;
  pthread_mutex_t m_lock;
};

#endif // SERVER_H
