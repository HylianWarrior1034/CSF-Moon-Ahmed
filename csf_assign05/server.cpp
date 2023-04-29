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

void server_chat_with_client(int clientfd, const char *webroot) {
	struct Message *req = NULL;
	rio_t in;

	rio_readinitb(&in, clientfd);

	req = message_read_request(&in);
	printf("got request for resource %s\n", req->resource);
	if (req) {
		server_generate_response(clientfd, req, webroot);
		message_destroy(req);
	}
}

void server_generate_response(int clientfd, struct Message *req, const char *webroot) {
	char *filename = NULL;

	/* only GET requests are supported */
	if (strcmp(req->method, "GET") != 0) {
		server_generate_text_response(clientfd, "403", "Forbidden",
			"only GET requests are allowed");
		goto response_done;
	}

	/*
	 * for now, assume all requests are for files
	 * in the webroot
	 */
	filename = concat(webroot, req->resource);
	struct stat s;
	if (stat(filename, &s) < 0) {
		server_generate_text_response(clientfd, "404", "Not Found",
			"requested resource does not exist");
		goto response_done;
	}

	/* write response line */
	writestr(clientfd, "HTTP/1.1 200 OK\r\n");

	/* write Content-Type and Content-Length headers */
	const char *content_type = server_determine_content_type(filename);
	if (content_type) {
		writestr(clientfd, "Content-Type: ");
		writestr(clientfd, content_type);
		writestr(clientfd, "\r\n");
	}
	writestr(clientfd, "Content-Length: ");
	writelong(clientfd, (long) s.st_size);
	writestr(clientfd, "\r\n");

	writestr(clientfd, "\r\n");

	/* send file data */
	int fd = Open(filename, O_RDONLY, 0);
	copyto(fd, clientfd);
	close(fd);

response_done:
	free(filename);
}

void server_generate_text_response(int clientfd, const char *response_code,
	const char *reason, const char *msg) {
	writestr(clientfd, "HTTP/1.1 ");
	writestr(clientfd, response_code);
	writestr(clientfd, " ");
	writestr(clientfd, reason);
	writestr(clientfd, "\r\n");
	/* could generate headers... */
	writestr(clientfd, "\r\n");
	writestr(clientfd, msg);
}

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

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

  struct Connection *client_conenction = (Connection *) arg;

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

    struct Connection client_connection(clientfd);

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, worker, &client_connection) != 0)
    {

    }

    // finish while loop later after i figure out room functionality

  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
}
