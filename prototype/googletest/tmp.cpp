TEST(Response_test, POST)
{
  const char* configfile = "testcase/023_postfile.conf";
  Config config_;
  parseConfig(configfile, config_);

  // 0153
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/index.html";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["content-length"] = "10";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/index.html", Response.targetFilePath);
  }
  // 0130
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/index.html";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/index.html", Response.targetFilePath);
  }
  // 0131
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(201, ResponseStatus);
    // EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/index.htm", Response.targetFilePath);
  }
  // 0132
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/root.html";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(403, ResponseStatus);
  }
  // 0133
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/root/index.html";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(403, ResponseStatus);
  }
  // 0134
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/001/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/001/index.htm", Response.targetFilePath);
  }
  // 0135
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/www/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/alias/index.htm", Response.targetFilePath);
  }
  // 0136
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/auth/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(401, ResponseStatus);
  }
  // 0137
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/auth/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";
    client_.hmp.headers_["authorization"] = "Basic dXNlcjpwYXNzd29yZA==";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(204, ResponseStatus);
    EXPECT_EQ("/tmp/webserv/POST/auth/index.htm", Response.targetFilePath);
  }  
  // 0138
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/index.html";
    client_.hmp.headers_["host"] = "127.0.0.1";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(403, ResponseStatus);
  }
  // 0139
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/none/index.htm";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(403, ResponseStatus);
  }
  // 0140
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/limit/POSTdata.txt";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["transfer-encoding"] = "chunked";
    client_.body = "01234567890";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(413, ResponseStatus);
  }
  // 0141
  {
    Client client_;
    client_.port = 8080;
    client_.host = "*";
    client_.hmp.method_ = httpMessageParser::POST;
    client_.hmp.absolutePath_ = "/limit/POSTdata.txt";
    client_.hmp.headers_["host"] = "127.0.0.1";
    client_.hmp.headers_["content-length"] = "11";
    client_.body = "01234567890";

    Response Response(client_, config_);
    int ResponseStatus = Response.ResponseStatus;

    EXPECT_EQ(413, ResponseStatus);
  }  
}