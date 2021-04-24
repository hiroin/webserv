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
  r.setRecvData("7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n\r\n");
  r.cutOutRecvDataToEol();
  std::cout << r.getExtractedData() << std::endl;
  r.cutOutRecvDataBySpecifyingBytes(100);

  getChunked gc;
  gc.setClientBody(&clientBody);
  gc.setRecvData(&r);

  return 0;
}
