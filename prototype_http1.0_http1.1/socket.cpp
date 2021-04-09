#include "socket.hpp"

unsigned int ft_htonl(unsigned int x)
{
  if (__BYTE_ORDER == __BIG_ENDIAN)
  {
    return x;
  }
  else
  {
    return (
    ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
    (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24)) \
    );
  }
}

unsigned int ft_htons(unsigned short x)
{
  if (__BYTE_ORDER == __BIG_ENDIAN)
  {
    return x;
  }
  else
  {
    return(((unsigned short int) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))));
  }
}

void Socket::set_listenfd() {
  this->listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->listenfd == -1) {
    std::cout << "socket() failed." << std::endl;
    exit(1);
  }
}

void Socket::set_sockaddr_in() {
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));

  this->serv_addr.sin_family = AF_INET;
  this->serv_addr.sin_addr.s_addr = ft_htonl(INADDR_ANY);
  this->serv_addr.sin_port = ft_htons(this->port);
}

int Socket::set_socket() {
  Socket::set_listenfd();
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
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
    std::cout << "setsockopt() failed." << std::endl;
    close(listenfd);
    return -1;
  }

  Socket::set_sockaddr_in();
  if (bind(this->listenfd, (struct sockaddr*)&this->serv_addr, sizeof(this->serv_addr)) == -1) {
    std::cout << "bind() failed.(" << errno << ")" << std::endl;
    close(this->listenfd);
    return -1;
  }

  if (listen(this->listenfd, SOMAXCONN) == -1) {
    std::cout << "listen() failed." << std::endl;
    close(this->listenfd);
    return -1;
  }

  // ノンブロッキングのソケットに変更
  int flags = fcntl(this->listenfd, F_GETFL);
  if(-1 == flags)
  {
    std::cout << "fcntl() failed." << std::endl;
    close(this->listenfd);
  }
  int result = fcntl(this->listenfd, F_SETFL, flags | O_NONBLOCK);
  if(-1 == result)
  {
    std::cout << "fcntl() failed." << std::endl;
    close(this->listenfd);
  }

  return 0;
}