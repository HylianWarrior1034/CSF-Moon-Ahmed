#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  conn.connect(server_hostname, server_port);

  Message msg;
  msg.tag = TAG_RLOGIN;
  msg.data = username;

  if (!conn.send(msg))
  {
    std::cerr << "Error: Could not send messge.\n";
    return 2;
  }

  conn.receive(msg);

  if (msg.tag != TAG_OK)
  {
    std::cerr << "Error: server denied receiver login.\n";
    return 3;
  }

  // TODO: connect to server

  msg.tag = TAG_JOIN;
  msg.data = room_name

      if (!conn.send(msg))
  {
    std::cerr << "Error: Could not send messge.\n";
    return 2;
  }

  conn.receive(msg);

  if (msg.tag != TAG_OK)
  {
    std::cerr << "Error: server denied receiver login.\n";
    return 3;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  std::string room;
  std::string sender;
  std::string msg_text;

  while (1)
  {
    conn.receive(msg);
    if (msg.tag == TAG_DELIVERY)
    {
      std::stringstream ss(msg.data);
      std::getline(ss, room, " ");
      std::getline(ss, sender, " ");
      std::getline(ss, msg_text, " ");
      if (room == room_name)
      {
        std::cout << sender << ": " << msg_text << std::endl;
      }
    }
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  return 0;
}
