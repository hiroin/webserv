#include "util.hpp"

size_t		ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		i++;
		str++;
	}
	return (i);
}

// char *int10ToChar16(unsigned long ul)
// {
//   char *buf;
//   if (!(buf = (char *)malloc(1024)))
//     return (NULL);

//   char *strPtr = &buf[sizeof(buf) - 1];

//   *strPtr-- = 0;
//   if (ul == 0)
//   {
//     *strPtr = '0';
//     return strPtr;
//   }
//   while (ul)
//   {
//     *strPtr = ul % 16;
//     if (*strPtr < 10)
//       *strPtr += '0';
//     else
//       *strPtr += 'A' - 10;
//     if (ul /= 16)
//       strPtr--;
//   }
//   return strPtr;
// }

std::string int10ToChar16(unsigned long ul)
{
  char *buf;
  if (!(buf = (char *)malloc(1024)))
    return (NULL);

  char *strPtr = &buf[sizeof(buf) - 1];

  *strPtr-- = 0;
  if (ul == 0)
  {
    free(buf);
    return "0";
  }
  while (ul)
  {
    *strPtr = ul % 16;
    if (*strPtr < 10)
      *strPtr += '0';
    else
      *strPtr += 'A' - 10;
    if (ul /= 16)
      strPtr--;
  }
  std::string str = std::string(strPtr);
  free(buf);
  return str;
}

// int main()
// {
//   printf("%s\n", int10ToChar16(0));
//   printf("%s\n", int10ToChar16(9));
//   printf("%s\n", int10ToChar16(10));
//   printf("%s\n", int10ToChar16(17));
//   printf("%s\n", int10ToChar16(104));
// }

// int main()
// {
//   std::cout << int10ToChar16(0) << std::endl;
//   std::cout << int10ToChar16(17) << std::endl;
//   const char *tmp = int10ToChar16(17).c_str();
// }