#include "../../getChunked.hpp"
#include "../../recvData.hpp"

/*
clang++ -fsanitize=address -g ../../getChunked.cpp ../../recvData.cpp main.cpp -std=c++98
./a.out [ファイル名]
*/

int main(int argc, char* argv[])
{
  std::string clientBody;
  recvData r;
  getChunked gc;
  // r.setRecvData("7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n");
  r.setRecvData("f\r\n123456789012345\r\nf\r\n123456789012345\r\n0\r\n");
  // r.setRecvData("8\r\nMozilla\r\n0\r\n");
  gc.setClientBody(&clientBody);
  gc.setRecvData(&r);
  gc.parseChunkedData();
  std::cout << clientBody << std::endl;
  return 0;
}
