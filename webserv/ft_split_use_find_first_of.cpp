#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> ft_split_use_find_first_of(std::string s, std::string d)
{
  std::vector<std::string> ret;
  for(size_t i = 0, n; i <= s.length(); i = n + 1)
  {
    n = s.find_first_of(d, i);
    if(n == std::string::npos)
      n = s.length();
    std::string tmp = s.substr(i , n - i);
    if (tmp == "")
      continue;
    ret.push_back(tmp);
  }
  return ret;
}

#ifdef TEST_FT_SPLIT
int main()
{
  std::vector<std::string> ret;
  ret = ft_split_use_find_first_of("a \tb c"," \t");
  for (std::vector<std::string>::iterator itr = ret.begin(); itr != ret.end(); itr++)
  {
    std::cout << *itr << std::endl;
  }
}
#endif