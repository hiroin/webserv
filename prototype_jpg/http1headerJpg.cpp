#include "http1headerJpg.hpp"
#include <iostream>

std::vector<std::string>& HTTP1_Header_Jpg::make_response404() {
  static std::vector<std::string> header ={ 
    "HTTP/1.1 404 Not Found\r\n", 
    "Content-Type: text/html; charset=UTF-8\r\n",
    "Connection: close\r\n",
    "\r\n",
    "<html><body><h1>404 Not found</h1><p>The requested URL was not found on this server.</p><hr><address>Original Server</address></body></html>\r\n"
  };

  return header;
}

std::vector<std::string>& HTTP1_Header_Jpg::make_response200(int body_length) {
  std::ostringstream oss;
  oss << "Content-Length: " << body_length << "\r\n";

  static std::vector<std::string> header = {
    "HTTP/1.1 200 OK\r\n",
    "Content-Type: image/jpeg\r\n",
    oss.str(),
    "Connection: Keep-Alive\r\n",
    "\r\n"
  };

  return header;
}
