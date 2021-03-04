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
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[])
{
  std::string buf;
  std::string s = "12345";
  char fileName[] = "Screenshot_2.png";

  std::ifstream ifs(fileName, std::ios::in | std::ios::binary );

  //読込サイズを調べる。
  ifs.seekg(0, std::ios::end);
  long long int size = ifs.tellg();
  ifs.seekg(0);

  //読み込んだデータをchar型に出力する
  std::cout << size << std::endl;
  char *data = new char[size];
  ifs.read(data, size);

  // int i = 0;
  // while (i < size)
  // {
  //   s += data[i++];
  // }
  buf.clear();
  buf.assign(data, size);
  s += buf;
  std::cout << s.size() << std::endl;

  s.append("67890");
  std::string tmp = s.substr(5, size);

  std::ofstream fout;
  fout.open("file.png", std::ios::out|std::ios::binary|std::ios::trunc);
  fout.write(tmp.c_str(), size);
  //fout.write(s.c_str(), size);
}

