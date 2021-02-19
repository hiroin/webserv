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
#include <future>
#include <thread>
#include <sys/wait.h>

#include "socket.hpp"
#include "http1response.hpp"
#include "http1header.hpp"
#include "http1parser.hpp"
#include "http1headerCgi.hpp"

#include "util.hpp"

int http1(char *envp[])
{
  std::string executive_file = HTML_FILE;
  Socket *sock = new Socket(HTTP1_PORT);
  sock->set_socket();

  int body_length = 0;
  int is_file_exist;

  int accfd = -1;

  while (1) {
    // accept
    // https://linuxjm.osdn.jp/html/LDP_man-pages/man2/accept.2.html
    // addr 引き数は sockaddr 構造体へのポインターである。
    // この構造体には接続相手のソケットのアドレスが入っている。
    // addr が NULL の場合、 addr には何も入らない。
    // この場合、 addrlen は使用されず、この引き数は NULL にしておくべきである。
    accfd = accept(sock->get_listenfd(), (struct sockaddr*)NULL, NULL);

    //初期化
    if (accfd == -1) {
        continue;
    }
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    std::string recv_str = "";
    ssize_t read_size = 0;

    // \r\n\r\nが来るまでメッセージ受信
    do {
      read_size = recv(accfd, buf, sizeof(buf) - 1, 0);
      if (read_size == -1) {
          std::cout << "read() failed." << std::endl;
          std::cout << "ERROR: " << errno << std::endl;
          close(accfd);
          accfd = -1;
          break;
      }
      if (read_size > 0) {
          recv_str.append(buf);
      }
      if (recv_str.find(std::string("\r\n\r\n")) != std::string::npos) {
          break;
      }
    } while (read_size > 0); 

    //リクエストされたパスを取得する
    std::string path = "", path_string = "";
    std::string exe = executive_file;
    std::size_t pos = exe.rfind('/');
    if (pos != std::string::npos) {
        exe = exe.substr(pos + 1);
    }
    path_string.clear();
    // GET /index.html HTTP/1.1
    // の/index.htmlの部分を取り出す
    path = HTTP1_Parser::get_requestline_path(buf);
    path_string = HTTP1_Parser::argv_path_analyzer(path, executive_file.c_str(), exe.c_str());
    std::cout << "path_string : " << path_string << std::endl;

    //.phpを発見したらcgiの処理へ移行
    if (path_string.find(".php") != std::string::npos)
    {
      //HTTPレスポンスを作成する
      std::string server_response;
      std::vector<std::string> header = HTTP1_Header_Cgi::make_response200();
      int header_size = header.size();
      for (int i = 0; i < header_size; i++) {
        server_response.append(header[i].c_str());
      }
      //ソケットディスクリプタにHTTPレスポンス内容を書き込む
      if (send(accfd, server_response.c_str(), server_response.length(), 0) == -1) {
          std::cout << "write() failed." << std::endl;
      }
      
      //CGIを実行する
      int r;
     	int p[2];
      char buf[1024];
      ssize_t count;

      if (pipe(p) < 0)
        exit(1);
      r = fork();
      if (r < 0)
        exit(1);
      if (r == 0) {
         (void) close(p[0]);
         (void) dup2(p[1], 1);
         (void) close(p[1]);
        //CGIを実行する子プロセス
        //ケース1 引数で渡す
        if (0) {
          char *argv[5];
          argv[0] = strdup("/usr/bin/php-cgi");
          argv[1] = strdup("/home/user42/42/tmp/php-cgi/test.php");
          argv[2] = strdup("name=ap2c9w");
          argv[3] = strdup("pass=dummy");
          argv[4] = NULL;
          (void) execve(argv[0], argv, envp);
        } else if (0) {
        // ケース2 環境変数で渡す
        // たまに、php-cgiがヘッダを出力した時点でEOLを返すらしくて、readが38文字で止まる
          char *argv[3];
          argv[0] = strdup("/usr/bin/php-cgi");
          argv[1] = strdup("/home/user42/42/tmp/php-cgi/test.php");
          argv[2] = NULL;
          char *env[5];
          env[0] = strdup("REQUEST_METHOD=GET");
          env[1] = strdup("REDIRECT_STATUS=200");
          env[2] = strdup("SCRIPT_FILENAME=/home/user42/42/tmp/php-cgi/test.php");
          env[3] = strdup("QUERY_STRING=name=ap2c9w");
          env[4] = NULL;
          (void) execve(argv[0], argv, env);
        } else if (1) {
          // ケース3 標準入力で渡す
          std::string str = "name=ap2c9w";
          char *argv[3];
          argv[0] = strdup("/usr/bin/php-cgi");
          argv[1] = strdup("/home/user42/42/tmp/php-cgi/test.php");
          argv[2] = NULL;
          char *env[6];
          env[0] = strdup("REQUEST_METHOD=POST");
          env[1] = strdup("REDIRECT_STATUS=200");
          env[2] = strdup("SCRIPT_FILENAME=/home/user42/42/tmp/php-cgi/test.php");
          env[3] = strdup("CONTENT_LENGTH=11");
          env[4] = strdup("CONTENT_TYPE=application/x-www-form-urlencoded");
          env[5] = NULL;

          int p2[2];
          pipe(p2);
          write(p2[1], str.c_str(), str.size());
          dup2(p2[0], 0);
          (void) execve(argv[0], argv, env);
        }
      }
      (void) close(p[1]);
      count = read(p[0], buf, 1024);
      std::cout << "count : " << count << std::endl;

      // send(accfd, "7\r\n", 3, 0);
      // send(accfd, "Mozilla\r\n", 9, 0);
      // send(accfd, "0\r\n", 3, 0);
      // send(accfd, "\r\n", 2, 0);

      // send(accfd, "9\r\n", 3, 0);
      // send(accfd, "Mozilla\r\n", 9, 0);
      // send(accfd, "\r\n0\r\n", 5, 0);
      // send(accfd, "\r\n", 2, 0);

      const char *tmp = int10ToChar16(count).c_str();
      send(accfd, tmp, ft_strlen(tmp), 0);
      send(accfd, "\r\n", 2, 0);

      // char *tmp = int10ToChar16(count);
      // std::cout << tmp << std::endl;
      // send(accfd, tmp, ft_strlen(tmp), 0);
      // // free(tmp);
      // send(accfd, "\r\n", 2, 0);

      // send(accfd, "68\r\n", 4, 0);
      //ソケットディスクリプタにレスポンス内容を書き込む
      if(send(accfd, buf, count, 0) == -1){
          std::cout << "write() failed." << std::endl;
      }
      send(accfd, "\r\n0\r\n", 5, 0);
      send(accfd, "\r\n", 2, 0);
    }
    else
    {
      //取得したパスのファイルを開いて内容を取得する
      std::ifstream output_file(path_string.c_str());
      char line[256];
      is_file_exist = output_file.fail();
      body_length = 0;
      std::vector<std::string> message_body;
      while (output_file.getline(line, 256-1)) {
          body_length += strlen(line);
          message_body.push_back(std::string(line));
      }

      //HTTPレスポンスを作成する
      std::string server_response;
      std::vector<std::string> header = HTTP1_Response::make_header(3, body_length, is_file_exist, path);
      server_response = HTTP1_Response::make_response(header, message_body);
      std::cout << server_response << std::endl;

      //ソケットディスクリプタにレスポンス内容を書き込む
      if(send(accfd, server_response.c_str(), server_response.length(), 0) == -1){
          std::cout << "write() failed." << std::endl;
      }
      //使い終わったファイルのクローズ
      output_file.close();
    }
    close(accfd);
    accfd = -1;
  }
  close(sock->get_listenfd());
  return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    http1(envp);
    return 0;
}