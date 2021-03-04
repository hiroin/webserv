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

#include "socket.hpp"
#include "http1response.hpp"
#include "http1header.hpp"
#include "http1parser.hpp"
#include "recvData.hpp"

int http1()
{
  std::string executive_file = HTML_FILE;
  Socket *sock = new Socket(HTTP1_PORT);
  sock->set_socket();

  int body_length = 0;
  int is_file_exist;

  int accfd[MAX_SESSION];
  recvData recvData[MAX_SESSION];
  for (int i = 0; i < MAX_SESSION; i++) {
    accfd[i] = -1;
  }

  fd_set fds;

  int j = 0; // 動作確認用
  while (1) {
    std::cout << j++ << "回目\n";
    FD_ZERO(&fds);
    FD_SET(sock->get_listenfd(), &fds);
    int width = sock->get_listenfd() + 1;

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
        // if (!recvData[i].cutOutRecvDataBySpecifyingBytes(5))
        // {
        //   std::cout << "------------------------" << std::endl;
        //   std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
        //   std::cout << "------------------------" << std::endl;
        // }
        // if (recvData[i].cutOutRecvDataToEol()) {
        //   std::cout << "------------------------" << std::endl;
        //   std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
        //   std::cout << "------------------------" << std::endl;
        // }
        // ソケットのクローズ
        if (1) {


          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }
          if (recvData[i].cutOutRecvDataToEol()) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }

          if (recvData[i].cutOutRecvDataBySpecifyingBytes(18)) {
            std::cout << "------------------------" << std::endl;
            std::cout << "// extractedData_\n" << recvData[i].getExtractedData() << "]" << std::endl;
            std::cout << "------------------------" << std::endl;
          }

          recvData[i].clearData();
          close(accfd[i]);
          accfd[i] = -1;
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