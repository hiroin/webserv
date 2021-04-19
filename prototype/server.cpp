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

enum clientStatus {PARSE_STARTLINE, PARSE_HEADER, RESV_BODY, READ, WRITE, SEND, NUM_OF_CLIENTSTATUS};
struct client
{
  enum clientStatus status;
  client(){status = PARSE_STARTLINE;};
};

int http1()
{
  std::string executive_file = HTML_FILE;
  std::vector<Socket> servers;
  try
  {
    servers.push_back(Socket(6000, "*"));
    servers.push_back(Socket(6100, "localhost"));
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
  fd_set fds;
  for (int i = 0; i < MAX_SESSION; i++)
    accfd[i] = -1;

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

    for (int i = 0; i < MAX_SESSION; i++) {
      if(accfd[i] == -1)
      {
          continue;
      }
      if (FD_ISSET(accfd[i], &fds))
      {
        if (!recvData[i].recvFromSocket())
        {
          close(accfd[i]);
          accfd[i] = -1;
          continue;
        }
        {
          // デバッグ
          std::cout << "--recvData-----------------------------" << std::endl;
          std::cout << recvData[i].getRecvData();
          std::cout << "---------------------------------------" << std::endl;
        }        
      }
      if (clients[i].status == PARSE_STARTLINE)
      {
        if (recvData[i].cutOutRecvDataToEol() \
          && recvData[i].getExtractedData() != "")
        {
          // デバッグ
          std::cout << "--extractedData_----------------------" << std::endl;
          std::cout << recvData[i].getExtractedData() << std::endl;
          std::cout << "--------------------------------------" << std::endl;
          if (hmp[i].parseRequestLine(recvData[i].getExtractedData())
            && hmp[i].parseRequestTarget(hmp[i].getRequestTarget()))
          {
            {
              // デバッグ
              std::cout << "--startLine-----------------------" << std::endl;
              std::cout << "method = " << hmp[i].getMethod() << std::endl;
              std::cout << "request-target = " << hmp[i].getRequestTarget() << std::endl;
              std::cout << "HTTP-version = " << hmp[i].getHTTPVersion() << std::endl;
              std::cout << "absolute-path = " << hmp[i].getAbsolutePath() << std::endl;
              std::cout << "query = " << hmp[i].getQuery() << std::endl;
              std::cout << "----------------------------------" << std::endl;
            }
            clients[i].status = PARSE_HEADER;
          }
          else
          {
            // 400を返す
            close(accfd[i]);
            recvData[i].clearData();
            hmp[i].clearData();
            accfd[i] = -1;
            continue;
          }
        }
      }
      if (clients[i].status == PARSE_HEADER)
      {
        while (recvData[i].cutOutRecvDataToEol())
        {
          // \r\nのみが来た場合、MessageHeaderが終了
          if (recvData[i].getExtractedData() == "")
          {
            // デバッグ
            {
              std::cout << "--headers  -----------------------" << std::endl;
              std::map<std::string, std::string> headers = hmp[i].getHeaders();
              for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
              {
                std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
              }
              std::cout << "----------------------------------" << std::endl;
            }
            // 状態を最初に戻す
            clients[i].status = PARSE_STARTLINE;
            hmp[i].clearData();
            break;
          }
          else
          {
            hmp[i].parseHeader(recvData[i].getExtractedData());
          }
        }
      }
    }
  }
  close(servers[0].get_listenfd());
  close(servers[1].get_listenfd());
  return 0;
}

int main(int argc, char *argv[])
{
    http1();
    return 0;
}