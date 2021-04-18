#include "Socket.hpp"

void Socket::set_sockaddr_in()
{
  ft_memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  this->serv_addr.sin_family = AF_INET;
  if (this->host == "*")
    this->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  else
    this->serv_addr.sin_addr.s_addr = inet_addr(this->host.c_str());
  this->serv_addr.sin_port = htons(this->port);
}

void Socket::set_socket()
{
  this->listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->listenfd == -1)
    throw std::runtime_error("socket() failed.");
  int optval = 1;
  // SOL_SOCKET 
  // レベル。SO_REUSEADDRを使いたい場合、レベルはSOL_SOCKETを指定する。
  //
  // SO_REUSEADDR
  // bind(2) コールに与えられたアドレスが正しいかを判断するルールで、 ローカルアドレスの再利用を可能にする。
  // つまり AF_INET ソケットなら、そのアドレスにバインドされたアクティブな listen 状態のソケットが存在しない限り、バインドが行える。
  // listen 状態のソケットがアドレス INADDR_ANY で特定のポートにバインドされている場合には、 このポートに対しては、どんなローカルアドレスでもバインドできない。
  // 引き数はブール整数のフラグである。
  // https://linuxjm.osdn.jp/html/LDP_man-pages/man7/socket.7.html
  if (setsockopt(this->listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
  {
    close(this->listenfd);
    throw std::runtime_error("setsockopt() failed.");
  }
  Socket::set_sockaddr_in();
  if (bind(this->listenfd, (struct sockaddr*)&this->serv_addr, sizeof(this->serv_addr)) == -1)
  {
    close(this->listenfd);
    std::string error_message = this->host;
    error_message += " bind() failed.";
    error_message += strerror(errno);
    error_message += ".";
    throw std::runtime_error(error_message);
  }
  if (listen(this->listenfd, SOMAXCONN) == -1)
  {
    close(this->listenfd);
    throw std::runtime_error("listen() failed.");
  }
  // ノンブロッキングのソケットに変更
  if(fcntl(this->listenfd, F_SETFL, O_NONBLOCK) == -1)
  {
    close(this->listenfd);
    throw std::runtime_error("fcntl() failed.");
  }
}

Socket::Socket(int port_, std::string host_) : port(port_), host(host_)
{
  try
  {
    set_socket();
  }
  catch(const std::exception& e)
  {
    throw;
  }
}