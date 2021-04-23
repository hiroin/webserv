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
#include <string>
#include <vector>
#include <ctime>
#include <cstring>

#include "Socket.hpp"
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "parseConfig.hpp"
#include "ft.hpp"

int http1(Config& c)
{
  
  std::vector<Socket> servers;
  try
  {
    for (std::vector<s_ConfigServer>::iterator itr = c.configGlobal.servers.begin();
      itr != c.configGlobal.servers.end();
      itr++
    )
    {
      servers.push_back(Socket(itr->port, itr->host));
    }
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
        if (clients[i].status == SEND)
          FD_SET(clients[i].socketFd, &writeFds);
        clients[i].receivedData.setFd(clients[i].socketFd);
        if (maxFd < (clients[i].socketFd + 1))
          maxFd = clients[i].socketFd + 1;
      }
    }
    tvForSelect.tv_sec = SELECT_TIMEOUT;
    tvForSelect.tv_usec = 0;   
    if (select(maxFd, &readFds, &writeFds, NULL, &tvForSelect) == -1)
    // if (select(maxFd, &readFds, NULL, NULL, &tvForSelect) == -1)
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
        if (!clients[i].receivedData.recvFromSocket())
        {
          close(clients[i].socketFd);
          clients[i].socketFd = -1;
          continue;
        }
        {
          // デバッグ
          std::cout << "--recvData-----------------------------" << std::endl;
          std::cout << clients[i].receivedData.getRecvData();
          std::cout << "---------------------------------------" << std::endl;
        }        
      }
      if (clients[i].status == PARSE_STARTLINE)
      {
        if (clients[i].receivedData.cutOutRecvDataToEol() \
          && clients[i].receivedData.getExtractedData() != "")
        {
          // デバッグ
          std::cout << "--extractedData_-----------------------" << std::endl;
          std::cout << clients[i].receivedData.getExtractedData() << std::endl;
          std::cout << "---------------------------------------" << std::endl;
          if (clients[i].hmp.parseRequestLine(clients[i].receivedData.getExtractedData())
            && clients[i].hmp.parseRequestTarget(clients[i].hmp.getRequestTarget()))
          {
            {
              // デバッグ
              std::cout << "--startLine----------------------------" << std::endl;
              std::cout << "method = " << clients[i].hmp.getMethod() << std::endl;
              std::cout << "request-target = " << clients[i].hmp.getRequestTarget() << std::endl;
              std::cout << "HTTP-version = " << clients[i].hmp.getHTTPVersion() << std::endl;
              std::cout << "absolute-path = " << clients[i].hmp.getAbsolutePath() << std::endl;
              std::cout << "query = " << clients[i].hmp.getQuery() << std::endl;
              std::cout << "---------------------------------------" << std::endl;
            }
            clients[i].status = PARSE_HEADER;
          }
          else
          {
            ft_dummy_response(400, clients[i].socketFd);
            close(clients[i].socketFd);
            clients[i].receivedData.clearData();
            clients[i].hmp.clearData();
            clients[i].socketFd = -1;
            continue;
          }
        }
      }
      if (clients[i].status == PARSE_HEADER)
      {
        while (clients[i].receivedData.cutOutRecvDataToEol())
        {
          // \r\nのみが来た場合、MessageHeaderが終了
          if (clients[i].receivedData.getExtractedData() == "")
          {
            // デバッグ
            std::map<std::string, std::string> headers = clients[i].hmp.getHeaders();
            {
              std::cout << "--headers------------------------------" << std::endl;
              for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
              {
                std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
              }
              std::cout << "---------------------------------------" << std::endl;
            }
            if (int code = clients[i].hmp.isInvalidHeaderValue() != 200)
            {
              ft_dummy_response(code, clients[i].socketFd);
              // 状態を最初に戻す
              clients[i].status = PARSE_STARTLINE;
              clients[i].hmp.clearData();
              break;
            }
            if (clients[i].isNeedBody(headers))
            {
              clients[i].status = RESV_BODY;
            }
            else
            {
              ft_dummy_response(200, clients[i].socketFd);
              // 状態を最初に戻す
              clients[i].status = PARSE_STARTLINE;
              clients[i].hmp.clearData();
              break;
            }
          }
          else
          {
            clients[i].hmp.parseHeader(clients[i].receivedData.getExtractedData());
            std::map<std::string, std::string> headers = clients[i].hmp.getHeaders();
            if (clients[i].hmp.isIllegalValueOfHostHeader(clients[i].receivedData.getExtractedData()))
            {
              ft_dummy_response(400, clients[i].socketFd);
              close(clients[i].socketFd);
              clients[i].status = PARSE_STARTLINE;
              clients[i].receivedData.clearData();
              clients[i].hmp.clearData();
              clients[i].socketFd = -1;
              continue;
            }
          }
        }
      }
      if (clients[i].status == RESV_BODY)
      {
        if (clients[i].bChunked == true)
        {
          // チャンクのデータが取得できる関数を書く
        }
        else
        {
          if (clients[i].receivedData.cutOutRecvDataBySpecifyingBytes(ft_stoi(clients[i].hmp.headers_["content-length"])))
          {
            clients[i].body = clients[i].receivedData.getExtractedData();
            {
              // デバッグ
              std::cout << "--body---------------------------------" << std::endl;
              std::cout << clients[i].body << std::endl;
              std::cout << "---------------------------------------" << std::endl;
            }
          }
        }
        ft_dummy_response(200, clients[i].socketFd);
        // 状態を最初に戻す
        clients[i].status = PARSE_STARTLINE;
        clients[i].hmp.clearData();
        clients[i].body.clear();
        break;
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
  if (argc != 2)
  {
    std::cout << "Usage: ./server [configfile]" << std::endl;
    return 1;
  }
  Config c;
  // c.printConfig(); // Config内容の出力
  try
  {
    parseConfig(argv[1], c);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  http1(c);
  return 0;
}