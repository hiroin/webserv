#include <vector>
#include <string>
#include <iostream>

void ft_print_vector_string(std::vector<std::string> vs)
{
  for (std::vector<std::string>::iterator itr = vs.begin(); itr != vs.end(); itr++)
  {
    std::cout << *itr << std::endl;
  }
}
