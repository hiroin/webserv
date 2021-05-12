#include "Webserv.hpp"

Wevserv::Wevserv(Config& c) : c_(c), maxFd_(0)
{
  setupServers();

  int selectReturn;
  struct timeval tvForSelect;
  struct timeval nowTv;
  unsigned long mainLoopCounter = 0;
  while (1)
  {
    if (c.getDebugLevel() >= 1)
      std::cout << "[DEBUG]Loop : " << mainLoopCounter++ << "time" << std::endl;
    gettimeofday(&nowTv, NULL);
    for (int i = 0; i < MAX_SESSION; i++)
    {
      if (clients_[i].socketFd != -1)
      {
        if (nowTv.tv_sec - clients_[i].lastTvForClient.tv_sec > SESSION_TIMEOUT)
        {
          if (c.getDebugLevel() >= 1)
            std::cout << "[DEBUG]clients_[" << i << "] session timeout." << std::endl;
          clients_[i].initClient();
        }
      }
    }
    initFD();
    tvForSelect.tv_sec = SELECT_TIMEOUT;
    tvForSelect.tv_usec = 0;   
    if (select(maxFd_, &readFds_, &writeFds_, NULL, &tvForSelect) == -1)
      std::cout << "[EMERG]select() failed.(" << strerror(errno) << ")" << std::endl;
    for (std::vector<Socket>::iterator itrServer = servers_.begin();
      itrServer != servers_.end();
      itrServer++)
    {
      if (FD_ISSET(itrServer->get_listenfd(), &readFds_))
      {
        struct sockaddr_in clienSockaddrIn;
        socklen_t clienSockaddrInLen = sizeof(clienSockaddrIn);
        int acceptFd = accept(itrServer->get_listenfd(), \
                              (struct sockaddr*)&clienSockaddrIn, \
                              &clienSockaddrInLen);
        if (acceptFd == -1)
        {
          std::cout << "[EMERG]accept() failed." << std::endl;
          continue;
        }
        int r = fcntl(acceptFd, F_SETFL, O_NONBLOCK);
        if (r == -1)
        {
          std::cout << "[EMERG]fcntl() failed." << std::endl;
          close(acceptFd);
          continue;
        }
        if (c.getDebugLevel() >= 1)
          std::cout << "[DEBUG]accept[to " << itrServer->get_host() << ":" << itrServer->get_port() << " from " << ft_inet_ntos(clienSockaddrIn.sin_addr) << "]" << std::endl;
        bool limitOver = false;
        int limitOverI;
        for (int i = 0; i < MAX_SESSION; i++)
        {
          if (clients_[i].socketFd == -1)
          {
            clients_[i].socketFd = acceptFd;
            clients_[i].receivedData.setFd(clients_[i].socketFd);
            clients_[i].sc.setFd(clients_[i].socketFd);
            clients_[i].port = itrServer->get_port();
            clients_[i].host = itrServer->get_host();
            clients_[i].ip = ft_inet_ntos(clienSockaddrIn.sin_addr);
            clients_[i].lastTvForClient.tv_sec = nowTv.tv_sec;
            if (i > SESSION_LIMIT - 1)
            {
              limitOverI = i;
              limitOver = true;
            }
            break;
          }
        }
        if (limitOver)
        {
          std::cout << "[ERR]over MAX_SESSION." << std::endl;
          responseNot200(limitOverI, 503);
        }
      }
    }
    for (int i = 0; i < MAX_SESSION; i++) {
      if(clients_[i].socketFd == -1)
        continue;
      if (FD_ISSET(clients_[i].socketFd, &readFds_))
      {
        if (!clients_[i].receivedData.recvFromSocket())
        {
          clients_[i].initClient();
          continue;
        }
        debugPrintGetRecvData(i);
      }
      if (FD_ISSET(clients_[i].readFd, &readFds_))
      {
        if (!clients_[i].readDataFromFd.readFromFd())
        {
          clients_[i].initClient();
          continue;
        }
        debugPrintGetReadData(i);
      }
      if (clients_[i].status == PARSE_STARTLINE)
      {
        if (clients_[i].receivedData.cutOutRecvDataToEol() \
          && clients_[i].receivedData.getExtractedData() != "")
        {
          clients_[i].lastTvForClient.tv_sec = nowTv.tv_sec;
          debugPrintExtractedData(i);
          int code = clients_[i].hmp.parseRequestLine(clients_[i].receivedData.getExtractedData());
          if (code != 200)
            responseNot200(i, code);
          else if (clients_[i].hmp.parseRequestTarget(clients_[i].hmp.getRequestTarget()))
          {
            debugPrintStartLine(i);
            clients_[i].status = PARSE_HEADER;
          }
          else
            responseNot200(i, 400);
        }
      }
      if (clients_[i].status == PARSE_HEADER)
      {
        while (clients_[i].receivedData.cutOutRecvDataToEol())
        {
          // \r\nのみが来た場合、MessageHeaderが終了
          if (clients_[i].receivedData.getExtractedData() == "")
          {
            debugPrintHeaders(i);
            std::map<std::string, std::string> headers = clients_[i].hmp.getHeaders();
            if (int code = clients_[i].hmp.isInvalidHeaderValue() != 200)
            {
              responseNot200(i, code);
              clients_[i].initClient();
            }
            if (clients_[i].isNeedBody(headers))
            {
              clients_[i].status = RESV_BODY;
              debugPrintGetRecvData(i);
            }
            else
            {
              responses_[i] = new Response(clients_[i], c);
              debugPrintResponceData(i);
              clients_[i].responseCode = responses_[i]->ResponseStatus;
              if (clients_[i].status == READ)
              {
                clients_[i].readFd = responses_[i]->getTargetFileFd();
                clients_[i].readDataFromFd.setFd(clients_[i].readFd);
                coutLog(i);
              }
              else if (clients_[i].status == SEND)
              {
                clients_[i].responseMessege = responses_[i]->responseMessege;
                coutLog(i);
                delete responses_[i];
                clients_[i].sc.setSendData(const_cast<char *>(clients_[i].responseMessege.c_str()), responses_[i]->responseMessege.size());              
              }
              else
                std::cout << "[emerg] Irregularity status in Response" << std::endl;
            }
            break;
          }
          else
          {
            if (clients_[i].hmp.isIllegalValueOfHostHeader(clients_[i].receivedData.getExtractedData())
              || clients_[i].hmp.parseHeader(clients_[i].receivedData.getExtractedData()) == 400)
            {
              responseNot200(i, 400);
              continue;
            }
          }
        }
      }
      if (clients_[i].status == RESV_BODY)
      {
        if (clients_[i].bChunked == true)
        {
          clients_[i].gc.setRecvData(&clients_[i].receivedData);
          clients_[i].gc.setClientBody(&clients_[i].body);
          int code = clients_[i].gc.parseChunkedData();
          if (code == 200)
          {
            debugPrintRequestBody(i);
            while (clients_[i].receivedData.cutOutRecvDataToEol())
            {
              if (clients_[i].receivedData.getExtractedData() == "")
              {
                debugPrintHeaders(i);
                int code = clients_[i].hmp.isInvalidHeaderValue();
                if (code != 200)
                  responseNot200(i, code);
                break;
              }
              else
              {
                if (clients_[i].hmp.isIllegalValueOfHostHeader(clients_[i].receivedData.getExtractedData())
                  || clients_[i].hmp.parseHeader(clients_[i].receivedData.getExtractedData()) == 400)
                {
                  responseNot200(i, code);
                  break;
                }
              }
            }
          }
          else if (code == 400)
          {
            responseNot200(i, code);
            continue;
          }
        }
        else
        {
          if (clients_[i].receivedData.cutOutRecvDataBySpecifyingBytes(ft_stoi(clients_[i].hmp.headers_["content-length"])))
          {
            clients_[i].body = clients_[i].receivedData.getExtractedData();
            debugPrintRequestBody(i);
          }
        }
        // 未完成部分 PUTができたら稼働させる
        std::cout << "[DEBUG]PUT!" << std::endl;
        ft_dummy_response(200, clients_[i].socketFd);
        clients_[i].status = PARSE_STARTLINE;
        clients_[i].hmp.clearData();
        clients_[i].body.clear();
      }
      if (clients_[i].status == READ)
      {
        if (clients_[i].readDataFromFd.isCompleteRead())
        {
          close(clients_[i].readFd);
          clients_[i].readFd = -1;
          responses_[i]->AppendBodyOnResponseMessage(clients_[i].readDataFromFd.getReadData());
          debugPrintResponseMessege(i);
          clients_[i].readDataFromFd.clearData();
          clients_[i].responseMessege = responses_[i]->responseMessege;
          clients_[i].sc.setSendData(const_cast<char *>(clients_[i].responseMessege.c_str()), responses_[i]->responseMessege.size());
          clients_[i].status = SEND;
          delete responses_[i];
        }
      }
      if (FD_ISSET(clients_[i].socketFd, &writeFds_) && clients_[i].status == SEND)
      {
        try
        {
          bool isFinish = clients_[i].sc.SendMessage(1000000);
          if (isFinish)
          {
            if (isNotKeepConnectionCode(clients_[i].responseCode))
            {
              clients_[i].initClient();
            }
            else
            {
              clients_[i].status = PARSE_STARTLINE;
              clients_[i].hmp.clearData();
              clients_[i].body.clear();
            }
          }
        }
        catch(const std::exception& e)
        {
          clients_[i].initClient();
          std::cerr << e.what() << '\n';
        }
      }
    }
  }
}

Wevserv::~Wevserv()
{
  for (std::vector<Socket>::iterator itrServer = servers_.begin();
    itrServer != servers_.end();
    itrServer++)
  {
    close(itrServer->get_listenfd());
  }
  for (int i = 0; i < MAX_SESSION; i++)
  {
    clients_[i].initClient();
  }
}

// 第一引数と第二引数で与えられるポートとホストの組み合わせがportAndHostCombinationにいない場合trueを返す
bool Wevserv::notBeCreatedSocket(int port, std::string host, \
                                  std::vector<portAndHost>& portAndHostCombination)
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

void Wevserv::setupServers()
{
  try
  {
    std::vector<portAndHost> portAndHostCombination;
    for (std::vector<s_ConfigServer>::iterator itr = c_.configGlobal.servers.begin();
      itr != c_.configGlobal.servers.end();
      itr++
    )
    {
      if (notBeCreatedSocket(itr->port, itr->host, portAndHostCombination))
        servers_.push_back(Socket(itr->port, itr->host));
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
}

void Wevserv::initFD()
{
  FD_ZERO(&readFds_);
  FD_ZERO(&writeFds_);
  for (std::vector<Socket>::iterator itrServer = servers_.begin();
    itrServer != servers_.end();
    itrServer++)
  {
    FD_SET(itrServer->get_listenfd(), &readFds_);
    if (maxFd_ < (itrServer->get_listenfd() + 1))
      maxFd_ = itrServer->get_listenfd() + 1;
  }
  for (int i = 0; i < MAX_SESSION; i++)
  {
    if (clients_[i].socketFd != -1)
    {
      FD_SET(clients_[i].socketFd, &readFds_);
      if (clients_[i].status == SEND)
        FD_SET(clients_[i].socketFd, &writeFds_);
      if (maxFd_ < (clients_[i].socketFd + 1))
        maxFd_ = clients_[i].socketFd + 1;
    }
    if (clients_[i].readFd != -1)
    {
      FD_SET(clients_[i].readFd, &readFds_);
      if (maxFd_ < (clients_[i].readFd + 1))
        maxFd_ = clients_[i].readFd + 1;
    }
  }
}

bool Wevserv::isNotKeepConnectionCode(int code)
{
  switch (code)
  {
  case 400:
  case 500:
  case 501:
  case 502:
  case 503:
  case 504:
  case 505:
    return true;
  }
  return false;
}

void Wevserv::responseNot200(int i, int code)
{
  responses_[i] = new Response(code ,clients_[i], c_);
  if (clients_[i].status == READ)
  {
    clients_[i].readFd = responses_[i]->getTargetFileFd();
    clients_[i].readDataFromFd.setFd(clients_[i].readFd);
    clients_[i].responseCode = code;
    coutLog(i);
  }
  else if (clients_[i].status == SEND)
  {
    clients_[i].responseMessege = responses_[i]->responseMessege;
    coutLog(i);
    delete responses_[i];
    clients_[i].responseCode = code;
    clients_[i].sc.setSendData(const_cast<char *>(clients_[i].responseMessege.c_str()), responses_[i]->responseMessege.size());              
  }
  else
  {
    std::cout << "[emerg] Irregularity status in Response" << std::endl;
    delete responses_[i];
    std::string response = ft_make_dummy_response(400);
    clients_[i].sc.setSendData(const_cast<char *>(response.c_str()), response.size());
    clients_[i].responseCode = 400;
    clients_[i].status = SEND;
  }
}

void Wevserv::coutLog(int i)
{
  std::string log;
  log += clients_[i].ip;
  log += " [";
  log += getTime();
  log += "] ";
  log += ft_itos(clients_[i].responseCode);
  log += " \"";
  log += clients_[i].hmp.getMethodString();
  log += " ";
  log += clients_[i].hmp.getRequestTarget();
  log += " ";
  log += clients_[i].hmp.getHTTPVersion();
  log += "\" ";
  if (clients_[i].hmp.headers_["referer"] == "")
    log += "-";
  else
  {
    log += "\"";
    log += clients_[i].hmp.headers_["referer"];
    log += "\"";
  }
  log += " ";
  if (clients_[i].hmp.headers_["user-agent"] == "")
    log += "-";
  else
  {
    log += "\"";
    log += clients_[i].hmp.headers_["user-agent"];
    log += "\"";
  }
  std::cout << log << std::endl;
}

std::map<int, std::string> Wevserv::getMonth()
{
	std::map<int, std::string> month;
	month[0] = "Jan";
	month[1] = "Feb";
	month[2] = "Mar";
	month[3] = "Apr";
	month[4] = "May";
	month[5] = "Jun";
	month[6] = "Jul";
	month[7] = "Aug";
	month[8] = "Sep";
	month[9] = "Oct";
	month[10] = "Nov";
	month[11] = "Dec";
	return (month);
}

std::string Wevserv::ft_ito00(int n)
{
  std::string ret;
  if (n <= 9)
  {
    ret += '0';
    ret += '0' + n;
    return ret;
  }
  while (n > 0)
  {
    char c[2];
    c[0] = '0' + n % 10;
    c[1] = '\0';
    ret.insert(0, std::string(c));
    n /= 10;
  }
  return (ret);
}

std::string Wevserv::getTime()
{
  std::string timestamp;
	time_t timer;
	struct tm *gmt;
	time(&timer);
	gmt = gmtime(&timer);
	timestamp.append(ft_ito00(gmt->tm_mday) + "/");
	timestamp.append(getMonth()[gmt->tm_mon] + "/");
	timestamp.append(ft_ito00(gmt->tm_year + 1900) + " ");
	timestamp.append(ft_ito00(gmt->tm_hour) + ":" + ft_ito00(gmt->tm_min) + ":" + ft_ito00(gmt->tm_sec));
  return timestamp;
}

void Wevserv::debugPrintGetRecvData(int i)
{
  if (c_.getDebugLevel() >= 2)
  {
    std::cout << "--recvData-----------------------------" << std::endl;
    std::cout << clients_[i].receivedData.getRecvData();
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintGetReadData(int i)
{
  if (c_.getDebugLevel() >= 2)
  {
    std::cout << "--readData-----------------------------" << std::endl;
    std::cout << clients_[i].readDataFromFd.getReadData();
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintExtractedData(int i)
{
  if (c_.getDebugLevel() >= 2)
  {
    std::cout << "--extractedData_-----------------------" << std::endl;
    std::cout << clients_[i].receivedData.getExtractedData() << std::endl;
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintStartLine(int i)
{
  if (c_.getDebugLevel() >= 1)
  {
    std::cout << "--startLine----------------------------" << std::endl;
    std::cout << "method = " << clients_[i].hmp.getMethod() << std::endl;
    std::cout << "request-target = " << clients_[i].hmp.getRequestTarget() << std::endl;
    std::cout << "HTTP-version = " << clients_[i].hmp.getHTTPVersion() << std::endl;
    std::cout << "absolute-path = " << clients_[i].hmp.getAbsolutePath() << std::endl;
    std::cout << "query = " << clients_[i].hmp.getQuery() << std::endl;
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintHeaders(int i)
{
  if (c_.getDebugLevel() >= 1)
  {
    std::map<std::string, std::string> headers = clients_[i].hmp.getHeaders();
    std::cout << "--headers------------------------------" << std::endl;
    for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
      std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintResponceData(int i)
{
  if (c_.getDebugLevel() >= 1)
  {
    std::cout << "--responceData-------------------------" << std::endl;
    std::cout << "response_code  : " << responses_[i]->ResponseStatus << std::endl;
    std::cout << "file_path      : " << responses_[i]->targetFilePath << std::endl;
    std::cout << "file_length    : " << responses_[i]->getContentLength() << std::endl;
    std::cout << "open_fd        : " << responses_[i]->getTargetFileFd() << std::endl;
    std::cout << "client_status  : " << clients_[i].status << std::endl;
    std::cout << "responseMessege  " << std::endl << responses_[i]->responseMessege << std::endl;
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintRequestBody(int i)
{
  if (c_.getDebugLevel() >= 2)
  {
    std::cout << "--body---------------------------------" << std::endl;
    std::cout << clients_[i].body << std::endl;
    std::cout << "---------------------------------------" << std::endl;
  }
}

void Wevserv::debugPrintResponseMessege(int i)
{
  if (c_.getDebugLevel() >= 2)
  {
    std::cout << "--responseMessege----------------------" << std::endl;
    std::cout << responses_[i]->responseMessege << std::endl;
    std::cout << "---------------------------------------" << std::endl;
  }
}