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
  std::string chunksize_chunkext;
  std::string chunksize;
  r.setRecvData("7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n\r\n");
  r.cutOutRecvDataToEol();
  chunksize_chunkext = r.getExtractedData();
  std::cout << "chunksize_chunkext : " << chunksize_chunkext << std::endl;
  std::string::size_type pos = chunksize_chunkext.find(";");
  if (pos != std::string::npos)
    chunksize = chunksize_chunkext.substr(0, pos);
  else
    chunksize = chunksize_chunkext;
  std::cout << "chunksize          : " << chunksize << std::endl;


  r.cutOutRecvDataBySpecifyingBytes(100);

  getChunked gc;
  gc.setClientBody(&clientBody);
  gc.setRecvData(&r);

  return 0;
}
