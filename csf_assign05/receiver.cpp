
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

  conn.connect(server_hostname, server_port); // connect to server

  Message msg(TAG_RLOGIN, username);
  conn.send(msg); // send receiver logged in

  Message response_login;
  conn.receive(response_login); // receive server message

  if (response_login.tag == TAG_ERR) // if received message tag was err
  {
    std::cerr << response_login.data; // print error message and leave
    return 3;
  }
  else if (response_login.tag != TAG_OK) // if tag was anything other than ok,
  {
    std::cerr << "unexpected server response" << std::endl; // handle it then leave
    return 4;
  }

  // TODO: connect to server

  msg.tag = TAG_JOIN;
  msg.data = room_name;

  conn.send(msg); // send room name to server

  Message response_join;
  conn.receive(response_join);

  if (response_join.tag == TAG_ERR) // same error handling as above
  {
    std::cerr << response_join.data;
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
    if (!conn.receive(response)) // if receive from server fails
    {
      if (conn.get_last_result() == Connection::EOF_OR_ERROR) // if connection is closed, print EOF or ERROR
      {
        return -1;
      }
      else
      {
        std::cerr << "message invalid format" << std::endl; // else, the message is in invalid format
      }
    }
    else
    {
      if (response.tag == TAG_DELIVERY) // if message contains right tag,
      {
        std::stringstream ss(response.data);
        std::getline(ss, room, ':');
        std::getline(ss, sender, ':');
        std::getline(ss, msg_text, ':');

        if (room == room_name) // parse the message and see if message room matches receiver room
        {
          std::cout << sender << ": " << msg_text;
        }
      }
      else
      {
        std::cerr << "unexpected server response tag" << std::endl;
      }
    }
  }

  return 0;
}
