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

  // create Connection object and connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);

  // send a slogin message to server
  Message login_message(TAG_SLOGIN, username);
  if (!conn.send(login_message)) {
    std::cerr << "Unable to connect to server." << std::endl;
    return 1;
  }

  // retrieve response fro server
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

  // start main sender loop
  bool loop_exit_case = false;

  while (!loop_exit_case)
  {
    // read user input
    std::string line;
    std::getline(std::cin, line);
    Message send_message;

    // check if user inputted a command (i.e. something beginning with '/')
    if (line[0] == '/')
    {
      if (line.substr(0, 5).compare("/join") == 0)
      {
        // parse join command if user inputted "/join"
        send_message.tag = TAG_JOIN;
        send_message.data = line.substr(6);
      }
      else if (line.substr(0, 6).compare("/leave") == 0)
      {
        // set tag to "leave" if user inputted "/leave"
        send_message.tag = TAG_LEAVE;
        send_message.data = "";
      }
      else if (line.substr(0, 5).compare("/quit") == 0)
      {
        // set tag to "quit" if user inputted "/quit"
        send_message.tag = TAG_QUIT;
        send_message.data = "";
        loop_exit_case = true; // exit from loop
      }
      else
      {
        // handle error if the command is invalid
        std::cerr << "Error: invalid command" << std::endl;
        continue;
      }
    }
    else
    {
      // if user did not input a command, their inputted text will be sent witht the tage "sendall"
      send_message.tag = TAG_SENDALL;
      send_message.data = line;
    }

    // handle error if send fails
    if (!conn.send(send_message))
    {
      std::cerr << "Error: cannot send message." << std::endl;
      return 6;
    }

    // retreive ok response from server
    Message received_message;
    if (conn.receive(received_message))
    {
      if (received_message.tag == TAG_ERR)
      {
        std::cerr << received_message.data << std::endl;
      }
      else if (received_message.tag != TAG_OK)
      {
        std::cerr << "unexpected server response tag" << std::endl;
        return 7;
      }
    }
    else
    {
      if (!conn.is_open())
      {
        std::cerr << "could not receive due to EOF or Error" << std::endl;
        return 8;
      }
      else
      {
        std::cerr << "could not receive due to invalid format" << std::endl;
      }
    }
  }

  return 0;
}
