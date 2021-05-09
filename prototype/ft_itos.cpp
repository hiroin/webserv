#include "ft.hpp"

std::string ft_itos(int nu)
{
	std::string ret;
  if (nu == 0)
    ret = "0";
	while (nu > 0)
	{
		char c[2];
		c[0] = '0' + nu % 10;
		c[1] = '\0';
		ret.insert(0, std::string(c));
		nu /= 10;
	}
	return (ret);
}