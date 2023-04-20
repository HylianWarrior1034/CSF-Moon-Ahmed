#include <iostream>
#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
    : m_fd(-1), m_last_result(SUCCESS)
{
}

Connection::Connection(int fd)
    : m_fd(fd), m_last_result(SUCCESS)
{
  rio_readinitb(&m_fdbuf, fd);
  // TODO: call rio_readinitb to initialize the rio_t object
}

void Connection::connect(const std::string &hostname, int port)
{
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  int client_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  if (client_fd < 0)
  {
    std::cerr << "Could not connect to server" << std::endl;
    exit(1);
  }
  m_fd = client_fd;
  // handle error if client_fd is -1 or -2
  rio_readinitb(&m_fdbuf, client_fd);
}

Connection::~Connection()
{
  // TODO: close the socket if it is open
  this->close(); // call ::close()
}

bool Connection::is_open() const
{
  // TODO: return true if the connection is open
  if (m_last_result == EOF_OR_ERROR) // if last result is an error, connection is not open
  {
    return false;
  }
  return true;
}

void Connection::close()
{
  // TODO: close the connection if it is open
  if (this->is_open()) // if connection is open,
  {
    Close(m_fd); // close fd
  }
}

bool Connection::send(const Message &msg)
{
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  // make sure msg is formatted properly
  std::string message = msg.tag + ":" + msg.data + "\n";
  size_t length = message.length();
  ssize_t result = rio_writen(m_fd, message.c_str(), length);
  if (result != (ssize_t)length)
  {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg)
{
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string tag, data;
  char buf[Message::MAX_LEN];                                 // create buffer
  ssize_t n = rio_readlineb(&m_fdbuf, buf, Message::MAX_LEN); // read in message
  if (n <= 0)
  {
    msg.tag = TAG_EMPTY;
    m_last_result = EOF_OR_ERROR;
  }
  else
  {
    std::string message(buf);
    int index = message.find(":");        // parse the message
    msg.data = message.substr(index + 1); // before colon is tag
    msg.tag = message.substr(0, index);   // after colon is message
    if (msg.tag != TAG_ERR && msg.tag != TAG_OK && msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN && msg.tag != TAG_JOIN && msg.tag != TAG_LEAVE &&
        msg.tag != TAG_SENDALL && msg.tag != TAG_SENDUSER && msg.tag != TAG_QUIT && msg.tag != TAG_DELIVERY && msg.tag != TAG_EMPTY)
    {
      m_last_result = INVALID_MSG; // check tag of message
      return false;
    }
    m_last_result = SUCCESS; // change m_last_result to success if message is valid format
    return true;
  }
  return false;
}
