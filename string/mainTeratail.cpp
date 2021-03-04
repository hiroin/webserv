#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[], char *envp[])
{
  std::string s;
  std::ifstream ifs("input.png", std::ios::in | std::ios::binary );
  std::string tmp;

  ifs.seekg(0, std::ios::end);
  long long int size = ifs.tellg();
  ifs.seekg(0);

  char *data = new char[size];
  ifs.read(data, size);

  // OK
  // long long int i = 0;
  // while (i < size)
  // {
  //   s += data[i++];
  // }

  // OK
  // std::ostringstream oss;
  // oss.write(data, size);
  // s += oss.str();
  tmp.clear();
  tmp.assign(data, size);
  s += tmp;
  std::cout << s.size() << std::endl;

  // NG
  // s = data;

  std::ofstream fout;
  fout.open("output.png", std::ios::out|std::ios::binary|std::ios::trunc);
  fout.write(s.c_str(), size);
}

