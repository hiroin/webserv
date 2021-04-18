#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "configure.hpp"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstdlib>
#include <errno.h>
#include "ft.hpp"

class Socket {
 public:
  Socket(int port_, std::string host_);
  int get_listenfd() const { return this->listenfd; }

 private:
  int listenfd;
  int port;
  std::string host; 
  struct sockaddr_in serv_addr;
  void set_socket();
  void set_sockaddr_in();
};

#endif