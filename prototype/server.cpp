#include "server.hpp"

// 第一引数と第二引数で与えられるポートとホストの組み合わせがportAndHostCombinationにいない場合trueを返す
static bool notBeCreatedSocket(int port, std::string host, std::vector<portAndHost>& portAndHostCombination)
{
  for (std::vector<portAndHost>::iterator itrPortAndHost = portAndHostCombination.begin();
    itrPortAndHost != portAndHostCombination.end();
    itrPortAndHost++
  )
  {
    if (port == itrPortAndHost->port && host == itrPortAndHost->host)
      return false;
  }
  portAndHost tmp;
  tmp.port = port;
  tmp.host = host;
  portAndHostCombination.push_back(tmp);
  return true;
}

static void setupServers(std::vector<Socket>& servers, Config& c)
{
  try
  {
    std::vector<portAndHost> portAndHostCombination;
    for (std::vector<s_ConfigServer>::iterator itr = c.configGlobal.servers.begin();
      itr != c.configGlobal.servers.end();
      itr++
    )
    {
      if (notBeCreatedSocket(itr->port, itr->host, portAndHostCombination))
        servers.push_back(Socket(itr->port, itr->host));
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
}

static void initFD(fd_set& readFds, fd_set& writeFds, int& maxFd, std::vector<Socket>& servers, Client* clients)
{
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
      if (maxFd < (clients[i].socketFd + 1))
        maxFd = clients[i].socketFd + 1;
    }
    if (clients[i].readFd != -1)
    {
      FD_SET(clients[i].readFd, &readFds);
      if (maxFd < (clients[i].readFd + 1))
        maxFd = clients[i].readFd + 1;
    }
  }
}

static bool isNotKeepConnectionCode(int code)
{
  switch (code)
  {
  case 400:
  case 500:
  case 501:
  case 502:
  case 504:
  case 505:
    return true;
  }
  return false;
}

int http1(Config& c)
{
  std::vector<Socket> servers;
  setupServers(servers, c);
  Client clients[MAX_SESSION];
  Response *responses[MAX_SESSION];
  fd_set readFds;
  fd_set writeFds;
  int maxFd = 0;
  int selectReturn;
  struct timeval tvForSelect;
  unsigned long mainLoopCounter = 0;
  while (1)
  {
    if (c.getDebugLevel() >= 1)
      std::cout << "[DEBUG]Loop : " << mainLoopCounter++ << "time" << std::endl;
    initFD(readFds, writeFds, maxFd, servers, clients);
    tvForSelect.tv_sec = SELECT_TIMEOUT;
    tvForSelect.tv_usec = 0;   
    if (select(maxFd, &readFds, &writeFds, NULL, &tvForSelect) == -1)
      std::cout << "[EMERG]select() failed.(" << strerror(errno) << ")" << std::endl;
    for (std::vector<Socket>::iterator itrServer = servers.begin();
      itrServer != servers.end();
      itrServer++)
    {
      if (FD_ISSET(itrServer->get_listenfd(), &readFds))
      {
        struct sockaddr_in clienSockaddrIn;
        socklen_t clienSockaddrInLen = sizeof(clienSockaddrIn);
        int acceptFd = accept(itrServer->get_listenfd(), (struct sockaddr*)&clienSockaddrIn, &clienSockaddrInLen);
        int r = fcntl(acceptFd, F_SETFL, O_NONBLOCK);
        if (r == -1)
        {
          std::cout << "[EMERG]fcntl() failed." << std::endl;
          close(acceptFd);
          continue;
        }
        if (c.getDebugLevel() >= 1)
          std::cout << "[DEBUG]accept[to " << itrServer->get_host() << ":" << itrServer->get_port() << " from " << ft_inet_ntos(clienSockaddrIn.sin_addr) << "]" << std::endl;
        bool limit_over = true;
        for (int i = 0; i < MAX_SESSION; i++)
        {
          if (clients[i].socketFd == -1)
          {
            clients[i].socketFd = acceptFd;
            clients[i].receivedData.setFd(clients[i].socketFd);
            clients[i].sc.setFd(clients[i].socketFd);
            clients[i].port = itrServer->get_port();
            clients[i].host = itrServer->get_host();
            clients[i].ip = ft_inet_ntos(clienSockaddrIn.sin_addr);
            limit_over = false;
            break;
          }
        }
        if (limit_over)
        {
          std::cout << "[ERR]over MAX_SESSION." << std::endl;
          close(acceptFd);
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
          clients[i].initClient();
          continue;
        }
        {
          // デバッグ
          std::cout << "--recvData-----------------------------" << std::endl;
          std::cout << clients[i].receivedData.getRecvData();
          std::cout << "---------------------------------------" << std::endl;
        }
      }
      if (FD_ISSET(clients[i].readFd, &readFds))
      {
        if (!clients[i].readDataFromFd.readFromFd())
        {
          clients[i].initClient();
          continue;
        }
        {
          // デバッグ
          std::cout << "--readData-----------------------------" << std::endl;
          std::cout << clients[i].readDataFromFd.getReadData();
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
          int code = clients[i].hmp.parseRequestLine(clients[i].receivedData.getExtractedData());
          if (code != 200)
          {
            responses[i] = new Response(code ,clients[i], c);
            clients[i].responseCode = code;
            if (clients[i].status == READ)
            {
              clients[i].readFd = responses[i]->getTargetFileFd();
              clients[i].readDataFromFd.setFd(clients[i].readFd);
            }
            else if (clients[i].status == SEND)
            {
              clients[i].responseMessege = responses[i]->responseMessege;
              delete responses[i];
              clients[i].sc.setSendData(const_cast<char *>(clients[i].responseMessege.c_str()), responses[i]->responseMessege.size());              
            }
            else
            {
              // ここにくるケースはないはず
              std::cout << "[emerg] Irregularity in PARSE_STARTLINE" << std::endl;
              delete responses[i];
              std::string response = ft_make_dummy_response(400);
              clients[i].sc.setSendData(const_cast<char *>(response.c_str()), response.size());
              clients[i].responseCode = 400;
              clients[i].status = SEND;
            }
          }
          else if (clients[i].hmp.parseRequestTarget(clients[i].hmp.getRequestTarget()))
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
            responses[i] = new Response(400 ,clients[i], c);
            if (clients[i].status == READ)
            {
              clients[i].readFd = responses[i]->getTargetFileFd();
              clients[i].readDataFromFd.setFd(clients[i].readFd);
              clients[i].responseCode = 400;
            }
            else if (clients[i].status == SEND)
            {
              clients[i].responseMessege = responses[i]->responseMessege;
              delete responses[i];
              clients[i].sc.setSendData(const_cast<char *>(clients[i].responseMessege.c_str()), responses[i]->responseMessege.size());              
            }
            else
            {
              // ここにくるケースはないはず
              std::cout << "[emerg] Irregularity in PARSE_STARTLINE" << std::endl;
              delete responses[i];

              std::string response = ft_make_dummy_response(400);
              clients[i].sc.setSendData(const_cast<char *>(response.c_str()), response.size());
              clients[i].responseCode = 400;
              clients[i].status = SEND;
            }
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
            clients[i].initClient();
            }
            if (clients[i].isNeedBody(headers))
            {
              clients[i].status = RESV_BODY;
              // {
              //   // デバッグ
              //   std::cout << "--recvData-----------------------------" << std::endl;
              //   std::cout << clients[i].receivedData.getRecvData();
              //   std::cout << "---------------------------------------" << std::endl;
              // }
            }
            else
            {
              // Responseクラスを組み込み
              responses[i] = new Response(clients[i], c);
              {
                // デバッグ
                std::cout << "--responceData-------------------------" << std::endl;
                std::cout << "response_code  : " << responses[i]->ResponseStatus << std::endl;
                std::cout << "file_path      : " << responses[i]->targetFilePath << std::endl;
                std::cout << "file_length    : " << responses[i]->getContentLength() << std::endl;
                std::cout << "open_fd        : " << responses[i]->getTargetFileFd() << std::endl;
                std::cout << "client_status  : " << clients[i].status << std::endl;
                std::cout << "responseMessege  " << std::endl << responses[i]->responseMessege << std::endl;
                std::cout << "---------------------------------------" << std::endl;
              }
              clients[i].responseCode = responses[i]->ResponseStatus;
              if (clients[i].status == READ)
              {
                clients[i].readFd = responses[i]->getTargetFileFd();
                clients[i].readDataFromFd.setFd(clients[i].readFd);
              }
              else if (clients[i].status == SEND)
              {
                clients[i].responseMessege = responses[i]->responseMessege;
                delete responses[i];
                clients[i].sc.setSendData(const_cast<char *>(clients[i].responseMessege.c_str()), responses[i]->responseMessege.size());              
              }
              // delete responses[i];
              // responses[i] = NULL;

              // ft_dummy_response(200, clients[i].socketFd);
              // 状態を最初に戻す
              // clients[i].status = PARSE_STARTLINE;
              // clients[i].bChunked = false;
              // clients[i].hmp.clearData();
            }
            break;
          }
          else
          {
            if (clients[i].hmp.isIllegalValueOfHostHeader(clients[i].receivedData.getExtractedData())
              || clients[i].hmp.parseHeader(clients[i].receivedData.getExtractedData()) == 400)
            {
              ft_dummy_response(400, clients[i].socketFd);
              clients[i].initClient();
              continue;
            }
          }
        }
      }
      if (clients[i].status == RESV_BODY)
      {
        if (clients[i].bChunked == true)
        {
          clients[i].gc.setRecvData(&clients[i].receivedData);
          clients[i].gc.setClientBody(&clients[i].body);
          int code = clients[i].gc.parseChunkedData();
          if (code == 200)
          {
            {
              // デバッグ
              std::cout << "--body---------------------------------" << std::endl;
              std::cout << clients[i].body << std::endl;
              std::cout << "---------------------------------------" << std::endl;
            }
            while (clients[i].receivedData.cutOutRecvDataToEol())
            {
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
                  clients[i].initClient();
                }
                break;
              }
              else
              {
                if (clients[i].hmp.isIllegalValueOfHostHeader(clients[i].receivedData.getExtractedData())
                  || clients[i].hmp.parseHeader(clients[i].receivedData.getExtractedData()) == 400)
                {
                  ft_dummy_response(400, clients[i].socketFd);
                  clients[i].initClient();
                  continue;
                }
              }
            }
          }
          else if (code == 400 || code == 500)
          {
            ft_dummy_response(code, clients[i].socketFd);
            clients[i].initClient();
            continue;
          }
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
      }
      if (clients[i].status == READ)
      {
        if (clients[i].readDataFromFd.isCompleteRead())
        {
          close(clients[i].readFd);
          clients[i].readFd = -1;          
          {
            // デバッグ
            std::cout << "--responseMessege(before append)--------" << std::endl;
            std::cout << responses[i]->responseMessege << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "--append body---------------------------" << std::endl;
            std::cout << clients[i].readDataFromFd.getReadData() << std::endl;
            std::cout << "----------------------------------------" << std::endl;
          }
          responses[i]->AppendBodyOnResponseMessage(clients[i].readDataFromFd.getReadData());
          {
            // デバッグ
            std::cout << "--responseMessege(after append)--------" << std::endl;
            std::cout << responses[i]->responseMessege << std::endl;
            std::cout << "---------------------------------------" << std::endl;
          }
          clients[i].readDataFromFd.clearData();
          clients[i].responseMessege = responses[i]->responseMessege;
          clients[i].sc.setSendData(const_cast<char *>(clients[i].responseMessege.c_str()), responses[i]->responseMessege.size());
          clients[i].status = SEND;
          delete responses[i];
        }
      }
      if (FD_ISSET(clients[i].socketFd, &writeFds) && clients[i].status == SEND)
      {
        try
        {
          bool isFinish = clients[i].sc.SendMessage(1024);
          if (isFinish)
          {
            if (isNotKeepConnectionCode(clients[i].responseCode))
            {
              clients[i].initClient();
            }
            else
            {
              clients[i].status = PARSE_STARTLINE;
              clients[i].hmp.clearData();
              clients[i].body.clear();
            }
          }
        }
        catch(const std::exception& e)
        {
          clients[i].initClient();
          std::cerr << e.what() << '\n';
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
  if (argc != 2 && argc != 3)
  {
    std::cout << "Usage: ./server [configfile]" << std::endl;
    return 1;
  }
  Config c;
  if (argc == 3 && (std::string(argv[2]) == "1" || std::string(argv[2]) == "2"))
  {
    switch (argv[2][0])
    {
    case '1':
      c.setDebugLevel(1);
      break;
    case '2':
      c.setDebugLevel(1);
      break;
    default:
      break;
    }
  }
  if (c.getDebugLevel() >= 1)
    c.printConfig();
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