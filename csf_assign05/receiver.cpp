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

  Message msg(TAG_RLOGIN, username);
  conn.send(msg);

  Message response_login;
  conn.receive(response_login);

  if (response_login.tag == TAG_ERR)
  {
    std::cerr << "Error: " << response_login.data << std::endl;
    return 3;
  }
  else if (response_login.tag != TAG_OK)
  {
    std::cerr << "unexpected server response" << std::endl;
    return 4;
  }

  // TODO: connect to server

  msg.tag = TAG_JOIN;
  msg.data = room_name;

  conn.send(msg);

  Message response_join;
  conn.receive(response_join);

  if (response_join.tag == TAG_ERR)
  {
    std::cerr << "Error: " << response_join.data << std::endl;
    return 3;
  }
  else if (response_join.tag != TAG_OK)
  {
    std::cerr << "unexpected server response" << std::endl;
    return 4;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  std::string room;
  std::string sender;
  std::string msg_text;

  while (1)
  {
    Message response;
    if (!conn.receive(response))
    {
      if (conn.get_last_result() == Connection::EOF_OR_ERROR)
      {
        return -1;
      }
      else
      {
        std::cerr << "message invalid format" << std::endl;
      }
    }
    else
    {
      if (response.tag == TAG_DELIVERY)
      {
        std::stringstream ss(response.data);
        std::getline(ss, room, ':');
        std::getline(ss, sender, ':');
        std::getline(ss, msg_text, ':');

        if (room == room_name)
        {
          std::cout << sender << ": " << msg_text << std::endl;
        }
        f
      }
      else
      {
        std::cerr << "unexpected server response tag" << std::endl;
      }
    }
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  return 0;
}
