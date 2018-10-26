#include "random_key.h"

#include <cstdlib>
#include <string>


std::string generate_random_key()
{
  static bool initalized = false;
  if (!initalized)
  {
    srand(time(0));
    initalized = true;
  }

  const char* hexdigits = "0123456789abcdef";

  std::string result(40, ' ');
  for (uint i = 0; i < 40; ++i)
    result[i] = hexdigits[random()>>8 & 0xf];
  return result;
}
