#include "http1headerCgi.hpp"
#include <iostream>

std::vector<std::string>& HTTP1_Header_Cgi::make_response200() {
  static std::vector<std::string> header = {
    "HTTP/1.1 200 OK\r\n",
    "Transfer-Encoding: chunked\r\n",
    "Connection: Keep-Alive\r\n",
    "\r\n"
  };

  return header;
}
