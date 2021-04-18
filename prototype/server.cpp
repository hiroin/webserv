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
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

#include "Socket.hpp"
#include "http1response.hpp"
#include "http1header.hpp"
#include "http1parser.hpp"
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"

struct client
{
  bool getStartline;
  bool getHeader;
};


int http1()
{
  std::string executive_file = HTML_FILE;
  std::vector<Socket> servers;
  try
  {
    // servers.push_back(Socket(HTTP1_PORT, "*"));
    servers.push_back(Socket(HTTP1_PORT, "127.0.0.1"));
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }

  int body_length = 0;
  int is_file_exist;

  int accfd[MAX_SESSION];
  recvData recvData[MAX_SESSION];
  HTTPMessageParser hmp[MAX_SESSION];
  client clients[MAX_SESSION];

  for (int i = 0; i < MAX_SESSION; i++) {
    clients[i].getStartline = false;
    clients[i].getHeader = false;
    accfd[i] = -1;
  }

  fd_set fds;

  int j = 0; // 動作確認用
  while (1) {
    std::cout << j++ << "回目\n";
    FD_ZERO(&fds);
    FD_SET(servers[0].get_listenfd(), &fds);
    int width = servers[0].get_listenfd() + 1;

    for (int i = 0; i < MAX_SESSION; i++) {
      if (accfd[i] != -1) {
        FD_SET(accfd[i], &fds);
        recvData[i].setSocketFd(accfd[i]);
        if (width < (accfd[i] + 1)) {
          width = accfd[i] + 1;
        }
      }
    }
    // std::cout << "before select() : " << width << '\n'; // 動作確認
    // select
    // https://linuxjm.osdn.jp/html/LDP_man-pages/man2/select.2.html
    if (select(width, &fds, NULL,NULL, NULL) == -1) {
      std::cout << "select() failed." << std::endl;
      break;
    }
    // std::cout << "after select() : " << width << '\n'; // 動作確認

    if (FD_ISSET(servers[0].get_listenfd(), &fds)) {
      int connfd = accept(servers[0].get_listenfd(), (struct sockaddr*)NULL, NULL);

      // ノンブロッキングのソケットに変更
      // int flags = fcntl(connfd, F_GETFL);
      // if(-1 == flags)
      // {
      //   std::cout << "fcntl() failed." << std::endl;
      //   close(connfd);
      //   break;
      // }
      // int result = fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
      // if(-1 == result)
      // {
      //   std::cout << "fcntl() failed." << std::endl;
      //   close(connfd);
      //   break;
      // }
      int result = fcntl(connfd, F_SETFL, O_NONBLOCK);
      if(-1 == result)
      {
        std::cout << "fcntl() failed." << std::endl;
        close(connfd);
        break;
      }

      std::cout << "accept" << std::endl;
      bool limit_over = true;
      for (int i = 0; i < MAX_SESSION; i++) {
        if (accfd[i] == -1) {
          accfd[i] = connfd;
          limit_over = false;
          break;
        }
      }
      if(limit_over) {
        close(connfd);
        std::cout << "over max connection." << std::endl;
      }
    }

    // accept
    // https://linuxjm.osdn.jp/html/LDP_man-pages/man2/accept.2.html
    // addr 引き数は sockaddr 構造体へのポインターである。
    // この構造体には接続相手のソケットのアドレスが入っている。
    // addr が NULL の場合、 addr には何も入らない。
    // この場合、 addrlen は使用されず、この引き数は NULL にしておくべきである。
    //accfd = accept(sock->get_listenfd(), (struct sockaddr*)NULL, NULL);
    for (int i = 0; i < MAX_SESSION; i++) {
      if(accfd[i] == -1) {
          continue;
      }
      if (FD_ISSET(accfd[i], &fds)) {
        if (!recvData[i].recvFromSocket()) {
          close(accfd[i]);
          accfd[i] = -1;
          continue;
        }
        if (1)
        {
          std::cout << "------------------------" << std::endl;
          std::cout << "// recvData\n" << recvData[i].getRecvData() << "]]]" << std::endl;
          std::cout << "------------------------" << std::endl;
        }
        if (1) {
          while (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << std::endl;
            std::cout << "------------------------" << std::endl;
            if (clients[i].getStartline == false && hmp[i].parseRequestLine(recvData[i].getExtractedData()))
            {
              std::cout << "method = " << hmp[i].getMethod() << std::endl;
              std::cout << "request-target = " << hmp[i].getRequestTarget() << std::endl;
              std::cout << "HTTP-version = " << hmp[i].getHTTPVersion() << std::endl;
              clients[i].getStartline = true;
            }
            // \r\nのみが来た場合、MessageHeaderが終了
            else if (clients[i].getHeader == false && recvData[i].getExtractedData() == "")
            {
              clients[i].getHeader = true;

              // 初期化
              recvData[i].clearData();
              hmp[i].clearData();
              clients[i].getStartline = false;
              clients[i].getHeader = false;
              close(accfd[i]);
              accfd[i] = -1;
            }
            else if (clients[i].getHeader == false && hmp[i].parseHeader(recvData[i].getExtractedData()))
            {
              std::map<std::string, std::string> headers = hmp[i].getHeaders();
              for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
              {
                std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
              }
            }
          }
          // recvData[i].clearData();
          // close(accfd[i]);
          // accfd[i] = -1;
        }
      }
    }
  }
  close(servers[0].get_listenfd());
  return 0;
}

int main(int argc, char *argv[])
{
    http1();
    return 0;
}