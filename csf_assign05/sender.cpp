#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);

  // TODO: send slogin message
  Message login_message(TAG_SLOGIN, username);
  conn.send(login_message);

  Message response_login;
  conn.receive(response_login);
  if (response_login.tag == TAG_ERR)
  {
    std::cerr << response_login.data << std::endl;
    return 3;
  }
  else if (response_login.tag != TAG_OK)
  {
    std::cerr << "unexpected server response" << std::endl;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  bool loop_exit_case = false;

  while (!loop_exit_case)
  {
    std::string line;
    std::getline(std::cin, line);
    Message send_message;

    if (line[0] == '/')
    {
      if (line.substr(0, 5).compare("/join") == 0)
      {
        send_message.tag = TAG_JOIN;
        send_message.data = line.substr(6);
      }
      else if (line.substr(0, 6).compare("/leave") == 0)
      {
        send_message.tag = TAG_LEAVE;
        send_message.data = "";
      }
      else if (line.substr(0, 5).compare("/quit") == 0)
      {
        send_message.tag = TAG_QUIT;
        send_message.data = "";
        loop_exit_case = true;
      }
      else
      {
        std::cerr << "Error: invalid command" << std::endl;
        continue;
      }
    }
    else
    {
      send_message.tag = TAG_SENDALL;
      send_message.data = line;
    }

    if (!conn.send(send_message))
    {
      std::cerr << "Error: cannot send message." << std::endl;
      return 6;
    }

    Message received_message;
    if (conn.receive(received_message))
    {
      if (response.tag == TAG_ERR)
      {
        std::cerr << received_message.data;
      }
      else if (received_message.tag != TAG_OK)
      {
        std::cerr << "unexpected server response tag";
        return 7;
      }
    }
    else
    {
      if (!conn.is_open())
      {
        std::cerr << "could not receive due to EOF or Error";
        return 8;
      }
      else
      {
        std::cerr << "could not receive due to invalid format";
      }
    }
  }

  return 0;
}
