#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <errno.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>

#include "Socket.hpp"
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "parseConfig.hpp"
#include "getChunked.hpp"
#include "ft.hpp"
#include "Response.hpp"

struct portAndHost
{
  int port;
  std::string host;
};

#endif