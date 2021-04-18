#include "http1header.hpp"
#include <iostream>

std::vector<std::string>& HTTP1_Header::make_response404() {
  static std::vector<std::string> header;
  header.insert(header.begin(), "HTTP/1.1 404 Not Found\r\n"); 
  header.insert(header.begin(), "Content-Type: text/html; charset=UTF-8\r\n"); 
  header.insert(header.begin(), "Connection: close\r\n"); 
  header.insert(header.begin(), "\r\n"); 
  header.insert(header.begin(), "<html><body><h1>404 Not found</h1><p>The requested URL was not found on this server.</p><hr><address>Original Server</address></body></html>\r\n"); 
  return header;
}

std::vector<std::string>& HTTP1_Header::make_response200(int body_length) {
  std::ostringstream oss;
  oss << "Content-Length: " << body_length << "\r\n";

  static std::vector<std::string> header;
  header.insert(header.begin(), "HTTP/1.1 200 OK\r\n"); 
  header.insert(header.begin(), "Content-Type: text/html; charset=UTF-8\r\n"); 
  header.insert(header.begin(), oss.str()); 
  header.insert(header.begin(), "Connection: Keep-Alive\r\n"); 
  header.insert(header.begin(), "\r\n"); 
  return header;
}

std::vector<std::string>& HTTP1_Header::make_response302(std::string path) {
  std::ostringstream oss;
  if (path != "") {
    oss << "Location: http://127.0.0.1:5000" << path.c_str() << "\r\n";
  }
  else {
    oss << "Location: http://127.0.0.1:5000\r\n";
  }

  static std::vector<std::string> header;
  header.insert(header.begin(), "HTTP/1.1 302 Found\r\n");
  header.insert(header.begin(), oss.str()); 
  header.insert(header.begin(), "\r\n"); 
  return header;
}