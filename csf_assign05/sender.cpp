#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
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
  Message msg;
  msg.tag = TAG_SLOGIN;
  msg.data = username;

  if (!conn.send(msg)) {
    std::cerr << "Error: sender login failed.";
    return 5;
  }

  conn.receive(msg);
  if (msg.tag != TAG_OK) {
    std::cerr << "Error: server denied login attempt.";
    return 5;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  bool loop_exit_case = false;

  while (!loop_exit_case) {
    std::string line;
    getline(std::cin, line);

    if (line[0] == '/') {
      std::string command = rtrim(line);
      if (command.compare("/join") == 0) {
        msg.tag = TAG_JOIN;
        msg.data = ltrim(line);
        if (!conn.send(msg)) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 6;
        }

        conn.receive(msg);
        if (msg.tag != TAG_OK) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 6;
        }
      } else if (command.compare("/leave") == 0) {
        msg.tag = TAG_LEAVE;
        msg.data = "";
        if (!conn.send(msg)) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 7;
        }

        conn.receive(msg);
        if (msg.tag != TAG_OK) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 7;
        }
      } else if (command.compare("/quit") == 0) {
        msg.tag = TAG_QUIT;
        msg.data = "";
        if (!conn.send(msg)) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 8;
        }

        conn.receive(msg);
        if (msg.tag != TAG_OK) {
          std::cerr << "Error: " << msg.data << std::endl;
          return 8;
        }
        loop_exit_case = true;
      } else {
        
      }
    } else {
      msg.tag = TAG_SENDALL;
      msg.data = line;
      if (!conn.send(msg)) {
        std::cerr << "Error: " << msg.data << std::endl;
        return 9;
      }
      conn.receive(msg);
      if (msg.tag != TAG_OK) {
        std::cerr << "Error: " << msg.data << std::endl;
        return 9;
      }
    }
  }

  return 0;
}
