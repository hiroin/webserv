#include "ft.hpp"

std::string ft_make_dummy_response(int code)
{
  std::string response;
  if (code == 200)
  {
    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 7\r\n";
    response += "\r\n";
    response += "200 OK\n";
  }
  if (code == 400)
  {
    response += "HTTP/1.1 400 Bad Request\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 16\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "400 Bad Request\n";
  }
  if (code == 500)
  {
    response += "HTTP/1.1 500 Internal Server Error\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 26\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "500 Internal Server Error\n";
  }   
  if (code == 501)
  {
    response += "HTTP/1.1 501 Not Implemented\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 16\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "501 Not Implemented\n";
  }
  return response;
}