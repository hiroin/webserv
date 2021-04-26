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
  gc.setClientBody(&clientBody);
  gc.setRecvData(&r);

  r.setRecvData("7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n");
  // r.setRecvData("f\r\n123456789012345\r\nf\r\n123456789012345\r\n0\r\n");
  // r.setRecvData("8\r\nMozilla\r\n0\r\n");
  // r.setRecvData("7\r\nMozilla\n9\rDeveloper\r\n7\nNetwork\r0\r\n");
  // r.setRecvData("8\r\nMozilla\r\n0\r\n");
  std::cout << gc.parseChunkedData() << std::endl;
  std::cout << clientBody << std::endl;

  clientBody.clear();
  gc.clear();

  r.setRecvData("7\r\n");
  gc.parseChunkedData();
  std::cout << "[[" << r.getRecvData() << "]]" << std::endl;
  r.setRecvData("Mozilla\r\n0\r\n");
  std::cout << "[[" << r.getRecvData() << "]]" << std::endl;
  gc.parseChunkedData();
  std::cout << "clientBody         : " << clientBody << std::endl;

  return 0;
}
