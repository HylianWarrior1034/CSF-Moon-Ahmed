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
  rio_readinitb(&m_fdbuf, m_fdbuf.rio_fd);
  // TODO: call rio_readinitb to initialize the rio_t object
}

void Connection::connect(const std::string &hostname, int port)
{
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  int client_fd = open_clientfd(hostname.c_str(), (char *)port);
  // handle error if client_fd is -1 or -2
  rio_readinitb(&m_fdbuf, m_fdbuf.rio_fd);
}

Connection::~Connection()
{
  // TODO: close the socket if it is open
  this->close();
}

bool Connection::is_open() const
{
  // TODO: return true if the connection is open
  ssize_t i = rio_writen(m_fd, "Ayo", 4);
  if (i == -1)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void Connection::close()
{
  // TODO: close the connection if it is open
  if (this->is_open())
  {
    Close(m_fd);
  }
}

bool Connection::send(const Message &msg)
{
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  // make sure msg is formatted properly
  ssize_t result = rio_writen(m_fd, msg.data.c_str(), msg.data.length());
  if (result == -1)
  {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  else
  {
    m_last_result = SUCCESS;
    return true;
  }
}

bool Connection::receive(Message &msg)
{
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buf[1000];
  ssize_t result = rio_readlineb(&m_fdbuf, buf, sizeof(buf));
  if (result == -1)
  {
    msg.tag = TAG_ERR;
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  else
  {
    msg.tag = TAG_OK;
    msg.data = buf;
    m_last_result = SUCCESS;
    return true;
  }
}
