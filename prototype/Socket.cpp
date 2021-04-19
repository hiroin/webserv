#include "Socket.hpp"

bool Socket::search_hostname_from_hostsfile(std::string hosts)
{
  std::vector<std::string> splitNewline = ft_split_use_find_first_of(hosts, "\n");
  // ft_print_vector_string(splitNewline);
  for (std::vector<std::string>::iterator itrNewline = splitNewline.begin();
    itrNewline != splitNewline.end();
    itrNewline++)
  {
    std::vector<std::string> splitSPHT = ft_split_use_find_first_of(*itrNewline, " \t");
    // ft_print_vector_string(splitSPHT);
    // 127.0.0.1 localhostのように要素が2つ以上あることを確認
    // 127.0.0.1だったり
    // localhostだったりするのはNG
    if (splitSPHT.size() >= 2)
    {
      for (std::vector<std::string>::iterator itrElement = splitSPHT.begin() + 1;
        itrElement != splitSPHT.end();
        itrElement++)
      {
        if (*itrElement == this->host)
        {
          std::vector<std::string> splitDot = ft_split_use_find_first_of(splitSPHT.at(0), ".");
          // aaa.bbb.ccc.dddのように.が3つある場合に、IPv4のアドレスと判断
          // ::1だったり、ff02::1だったりはIpv6
          if (splitDot.size() == 4)
          {
            this->host = splitSPHT.at(0);
            // std::cout << this->host << std::endl;
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool Socket::resolve_hostname()
{
  // /etc/hostsのopen
  int fd = open("/etc/hosts", O_RDONLY);
  if (fd == -1)
  {
    std::cerr << "Failed to open /etc/hosts." << std::endl;
    return false;
  }
  // /etc/hostsの読み込み
  std::string hosts;
  char buf[1048576];
  int r;
  ft_memset(buf, 0, sizeof(buf));
  while ((r = read(fd, buf, sizeof(buf) - 1)) > 0)
  {
    hosts.append(buf);
    ft_memset(buf, 0, sizeof(buf));
  }
  if (r == -1)
  {
    std::string errorMessage = "Failed to read /etc/hosts.";
    throw std::runtime_error(errorMessage);
  }
  r = close(fd);
  if (r == -1)
  {
    std::string errorMessage = "Failed to close /etc/hosts.(";
    errorMessage += strerror(errno); 
    errorMessage += ")"; 
    throw std::runtime_error(errorMessage);
  }
  // /etc/hostsからhost_を探す
  // あればhost_にセット
  return (search_hostname_from_hostsfile(hosts));
}

void Socket::set_sockaddr_in()
{
  ft_memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  this->serv_addr.sin_family = AF_INET;
  if (this->host == "*")
    this->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  else if(resolve_hostname())
    this->serv_addr.sin_addr.s_addr = inet_addr(this->host.c_str());
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