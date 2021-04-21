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
#include "Client.hpp"



int http1()
{
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
  Client clients[MAX_SESSION];
  fd_set readFds;
  fd_set writeFds;
  int maxFd = 0; // [やること]起動時fdの上限をチェックする
  // selectのタイムアウト用
  int selectReturn;
  struct timeval tvForSelect;
  long j = 0; // 動作確認用
  while (1)
  {
    std::cout << "[DEBUG]" << j++ << "回目\n";
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    for (std::vector<Socket>::iterator itrServer = servers.begin();
      itrServer != servers.end();
      itrServer++)
    {
      FD_SET(itrServer->get_listenfd(), &readFds);
      if (maxFd < (itrServer->get_listenfd() + 1))
        maxFd = itrServer->get_listenfd() + 1;
    }
    for (int i = 0; i < MAX_SESSION; i++)
    {
      if (clients[i].socketFd != -1)
      {
        FD_SET(clients[i].socketFd, &readFds);
        FD_SET(clients[i].socketFd, &writeFds);
        clients[i].recvData.setSocketFd(clients[i].socketFd);
        if (maxFd < (clients[i].socketFd + 1))
          maxFd = clients[i].socketFd + 1;
      }
    }
    tvForSelect.tv_sec = SELECT_TIMEOUT;
    tvForSelect.tv_usec = 0;   
    // if (select(maxFd, &readFds, &writeFds, NULL, &tvForSelect) == -1)
    if (select(maxFd, &readFds, NULL, NULL, &tvForSelect) == -1)
      std::cout << "select() failed.(" << strerror(errno) << ")" << std::endl;
    for (std::vector<Socket>::iterator itrServer = servers.begin();
      itrServer != servers.end();
      itrServer++)
    {
      if (FD_ISSET(itrServer->get_listenfd(), &readFds))
      {
        int acceptFd = accept(itrServer->get_listenfd(), (struct sockaddr*)NULL, NULL);
        int r = fcntl(acceptFd, F_SETFL, O_NONBLOCK);
        if (r == -1)
        {
          std::cout << "fcntl() failed." << std::endl;
          close(acceptFd);
          continue;
        }
        std::cout << "accept[" << itrServer->get_host() << ":" << itrServer->get_port() << "]" << std::endl;
        bool limit_over = true;
        for (int i = 0; i < MAX_SESSION; i++)
        {
          if (clients[i].socketFd == -1)
          {
            clients[i].socketFd = acceptFd;
            clients[i].port = itrServer->get_port();
            clients[i].host = itrServer->get_host();
            clients[i].socketFd = acceptFd;
            limit_over = false;
            break;
          }
        }
        if (limit_over)
        {
          close(acceptFd);
          std::cout << "over max connection." << std::endl;
        }
      }
    }
    for (int i = 0; i < MAX_SESSION; i++) {
      if(clients[i].socketFd == -1)
      {
          continue;
      }
      if (FD_ISSET(clients[i].socketFd, &readFds))
      {
        if (!clients[i].recvData.recvFromSocket())
        {
          close(clients[i].socketFd);
          clients[i].socketFd = -1;
          continue;
        }
        {
          // デバッグ
          std::cout << "--recvData-----------------------------" << std::endl;
          std::cout << clients[i].recvData.getRecvData();
          std::cout << "---------------------------------------" << std::endl;
        }        
      }
      if (clients[i].status == PARSE_STARTLINE)
      {
        if (clients[i].recvData.cutOutRecvDataToEol() \
          && clients[i].recvData.getExtractedData() != "")
        {
          // デバッグ
          std::cout << "--extractedData_----------------------" << std::endl;
          std::cout << clients[i].recvData.getExtractedData() << std::endl;
          std::cout << "--------------------------------------" << std::endl;
          if (clients[i].hmp.parseRequestLine(clients[i].recvData.getExtractedData())
            && clients[i].hmp.parseRequestTarget(clients[i].hmp.getRequestTarget()))
          {
            {
              // デバッグ
              std::cout << "--startLine-----------------------" << std::endl;
              std::cout << "method = " << clients[i].hmp.getMethod() << std::endl;
              std::cout << "request-target = " << clients[i].hmp.getRequestTarget() << std::endl;
              std::cout << "HTTP-version = " << clients[i].hmp.getHTTPVersion() << std::endl;
              std::cout << "absolute-path = " << clients[i].hmp.getAbsolutePath() << std::endl;
              std::cout << "query = " << clients[i].hmp.getQuery() << std::endl;
              std::cout << "----------------------------------" << std::endl;
            }
            clients[i].status = PARSE_HEADER;
          }
          else
          {
            std::string response400;
            response400 += "HTTP/1.1 400 Bad Request\r\n";
            response400 += "Content-Type: text/html\r\n";
            response400 += "Content-Length: 16\r\n";
            response400 += "Connection: close\r\n";
            response400 += "\r\n";
            response400 += "400 Bad Request\n\r\n";
            std::cout << response400 << std::endl;
            if (send(clients[i].socketFd, response400.c_str(), response400.length(), 0) == -1)
              std::cout << "send() failed." << std::endl;
            close(clients[i].socketFd);
            clients[i].recvData.clearData();
            clients[i].hmp.clearData();
            clients[i].socketFd = -1;
            continue;
          }
        }
      }
      if (clients[i].status == PARSE_HEADER)
      {
        while (clients[i].recvData.cutOutRecvDataToEol())
        {
          // \r\nのみが来た場合、MessageHeaderが終了
          if (clients[i].recvData.getExtractedData() == "")
          {
            // デバッグ
            {
              std::cout << "--headers  -----------------------" << std::endl;
              std::map<std::string, std::string> headers = clients[i].hmp.getHeaders();
              for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
              {
                std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
              }
              std::cout << "----------------------------------" << std::endl;
            }
            std::string response200;
            response200 += "HTTP/1.1 200 OK\r\n";
            response200 += "Content-Type: text/html\r\n";
            response200 += "Content-Length: 7\r\n";
            response200 += "\r\n";
            response200 += "200 OK\n\r\n";
            std::cout << response200 << std::endl;
            if (send(clients[i].socketFd, response200.c_str(), response200.length(), 0) == -1)
              std::cout << "send() failed." << std::endl;
            // 状態を最初に戻す
            clients[i].status = PARSE_STARTLINE;
            clients[i].hmp.clearData();
            break;
          }
          else
          {
            clients[i].hmp.parseHeader(clients[i].recvData.getExtractedData());
            // ヘッダのHostの値をチェックするロジックを追加
          }
        }
      }
    }
  }
  for (std::vector<Socket>::iterator itrServer = servers.begin();
    itrServer != servers.end();
    itrServer++)
  {
    close(itrServer->get_listenfd());
    close(itrServer->get_listenfd());
  }
  return 0;
}

int main(int argc, char *argv[])
{
    http1();
    return 0;
}