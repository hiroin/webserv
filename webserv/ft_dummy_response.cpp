#include "ft.hpp"
#include <sys/types.h>
#include <sys/socket.h>

void ft_dummy_response(int code, int socketFd)
{
  if (code == 200)
  {
    std::string response200;
    response200 += "HTTP/1.1 200 OK\r\n";
    response200 += "Content-Type: text/html\r\n";
    response200 += "Content-Length: 7\r\n";
    response200 += "\r\n";
    response200 += "200 OK\n";
    std::cout << response200 << std::endl;
    if (send(socketFd, response200.c_str(), response200.length(), 0) == -1)
      std::cout << "send() failed." << std::endl;
  }
  if (code == 400)
  {
    std::string response400;
    response400 += "HTTP/1.1 400 Bad Request\r\n";
    response400 += "Content-Type: text/html\r\n";
    response400 += "Content-Length: 16\r\n";
    response400 += "Connection: close\r\n";
    response400 += "\r\n";
    response400 += "400 Bad Request\n";
    std::cout << response400 << std::endl;
    if (send(socketFd, response400.c_str(), response400.length(), 0) == -1)
      std::cout << "send() failed." << std::endl;
  }
  if (code == 500)
  {
    std::string response;
    response += "HTTP/1.1 500 Internal Server Error\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 26\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "500 Internal Server Error\n";
    std::cout << response << std::endl;
    if (send(socketFd, response.c_str(), response.length(), 0) == -1)
      std::cout << "send() failed." << std::endl;
  }   
  if (code == 501)
  {
    std::string response;
    response += "HTTP/1.1 501 Not Implemented\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: 16\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "501 Not Implemented\n";
    std::cout << response << std::endl;
    if (send(socketFd, response.c_str(), response.length(), 0) == -1)
      std::cout << "send() failed." << std::endl;
  }  
}