#include "../../readData.hpp"

/*
clang++ -fsanitize=address -g ../../readData.cpp main.cpp -std=c++98
./a.out [ファイル名]
*/

int main(int argc, char* argv[])
{
  readData r;
  r.setFd(0);
  if (!r.readFromFd())
  {
    std::cerr << "read fail." << std::endl;
  }
  if (r.cutOutRecvDataToEol())
  {
    std::cout << r.getExtractedData() << std::endl;
  }
  if (r.cutOutRecvDataToEol())
  {
    std::cout << r.getExtractedData() << std::endl;
  }
  std::cout << r.getReadData() << std::endl;
  return 0;
}
