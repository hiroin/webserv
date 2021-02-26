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

#include "socket.hpp"
#include "http1response.hpp"
#include "http1header.hpp"
#include "http1parser.hpp"

int http1()
{
  std::string executive_file = HTML_FILE;
  Socket *sock = new Socket(HTTP1_PORT);
  sock->set_socket();

  int body_length = 0;
  int is_file_exist;

  int accfd[MAX_SESSION];
  for (int i = 0; i < MAX_SESSION; i++) {
    accfd[i] = -1;
  }

  fd_set fds;

  // 各accfd用string
  std::string recv_str[MAX_SESSION];

  // selectのタイムアウト用
  int selectReturn;
  struct timeval tv;
  tv.tv_sec = SELECT_TIMEOUT;
  tv.tv_usec = 0;  

  // クライアントからの接続タイムアウト用
  struct timeval clientTv[MAX_SESSION];
  struct timeval nowTv;

  int j = 0; // 動作確認用
  while (1) {
    std::cout << j++ << "回目\n";
    FD_ZERO(&fds);
    FD_SET(sock->get_listenfd(), &fds);
    int width = sock->get_listenfd() + 1;

    // ここに各fdごとのtimeout機能を実装
    gettimeofday(&nowTv, NULL);
    for (int i = 0; i < MAX_SESSION; i++) {
      if (accfd[i] != -1) {
        if (nowTv.tv_sec - clientTv[i].tv_sec > SESSION_TIMEOUT) {
          std::cout << "session " << i << " timeout." << std::endl;
          close(accfd[i]);
          accfd[i] = -1;
          continue;
        }
        FD_SET(accfd[i], &fds);
        if (width < (accfd[i] + 1)) {
          width = accfd[i] + 1;
        }
      }
    }
    // select
    // https://linuxjm.osdn.jp/html/LDP_man-pages/man2/select.2.html
    if ((selectReturn = select(width, &fds, NULL,NULL, &tv)) == -1) {
      std::cout << "select() failed." << std::endl;
      break;
    }
    // https://linuxjm.osdn.jp/html/LDP_man-pages/man2/select.2.html のタイムアウトを参照。
    // timeoutした後は初期化が必要
    tv.tv_sec = SELECT_TIMEOUT;
    tv.tv_usec = 0;       
    if (selectReturn == 0) {
      std::cout << "select() timeout." << std::endl;
      for (int i = 0; i < width; i++) {
        if (accfd[i] != -1) {
          close(accfd[i]);
          accfd[i] = -1;
        }
      }
      continue;
    }

    if (FD_ISSET(sock->get_listenfd(), &fds)) {
      int connfd = accept(sock->get_listenfd(), (struct sockaddr*)NULL, NULL);

      // ノンブロッキングのソケットに変更
      int flags = fcntl(connfd, F_GETFL);
      if(-1 == flags)
      {
        std::cout << "fcntl() failed." << std::endl;
        close(connfd);
        break;
      }
      int result = fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
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
          // acceptした時刻を記録 エラー処理書いてない
          gettimeofday(&clientTv[i], NULL);
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
        std::cout << "FD_ISSET(accfd[i], &fds)" << std::endl; // 動作確認

        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        ssize_t read_size = 0;

        // \r\n\r\nが来るまでメッセージ受信
        // 永遠にデータがこないときのために、timeoutを設けてこのfdをcloseする必要がある
        read_size = recv(accfd[i], buf, sizeof(buf) - 1, 0);
        // クライアントがtelnetして、そのまま何もせずにソケットを閉じると、graceful shutdownとなって、read_sizeが0になる
        // そのため、read_sizeだったら正常終了
        if (read_size == 0) {
          std::cout << "graceful shutdown." << std::endl;
          close(accfd[i]);
          accfd[i] = -1;
          recv_str[i].clear();
          continue;
        }
        // readの後はerrornoを起動して見てはいけないそうなので、
        // read_sizeが-1にの場合は何もしないで、closeはtimeoutにまかせるのがいいかもです。
        if (read_size == -1) {
          std::cout << "read() failed." << std::endl;
          std::cout << "ERROR: " << errno << std::endl;
          close(accfd[i]);
          accfd[i] = -1;
          recv_str[i].clear();
          continue;
        }
        if (read_size > 0) {
          recv_str[i].append(buf);
          std::cout << "------------------------" << std::endl;
          std::cout << "// recv_str\n" << recv_str[i] << std::endl;
          std::cout << "------------------------" << std::endl;
          // データを受信した時刻を記録 エラー処理書いてない
          gettimeofday(&clientTv[i], NULL);          
        }
        if (recv_str[i].find(std::string("\r\n\r\n")) != std::string::npos) {
          //リクエストされたパスを取得する
          std::string path = "", path_string = "";
          std::string exe = executive_file;
          std::size_t pos = exe.rfind('/');
          if (pos != std::string::npos) {
              exe = exe.substr(pos + 1);
          }
          path_string.clear();

          // 元のソースがなぜかget_requestline_path()にrecv_strではなくbufを与えているので…とりあえずstring recv_strをchar bufに変換
          memset(buf, 0, sizeof(buf));
          const char *buf = recv_str[i].c_str();

          // GET /index.html HTTP/1.1
          // の/index.htmlの部分を取り出す
          path = HTTP1_Parser::get_requestline_path(buf);
          path_string = HTTP1_Parser::argv_path_analyzer(path, executive_file.c_str(), exe.c_str());
          std::cout << "path_string : " << path_string << std::endl;
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
          //std::cout << server_response << std::endl;

          //ソケットディスクリプタにレスポンス内容を書き込む
          if(send(accfd[i], server_response.c_str(), server_response.length(), 0) == -1){
              std::cout << "write() failed." << std::endl;
          }

          // keep-alive対応でcloseしない。ソケットのクローズはgraceful shutdownか、timeoutにまかせる
          // プロトコルがhttp1.0の場合はソケットをcloseする
          // [task]ちゃんとヘッダを追加解析して条件分岐させること
          if (recv_str[i].find(std::string("HTTP/1.0")) != std::string::npos) {
            close(accfd[i]);
            accfd[i] = -1;
          }
          
          //使い終わったファイルのクローズ
          recv_str[i].clear();
          output_file.close();

        }
      }
    }
  }
  close(sock->get_listenfd());
  return 0;
}

int main(int argc, char *argv[])
{
    http1();
    return 0;
}