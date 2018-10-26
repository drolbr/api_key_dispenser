#include "base64.h"


std::string base64(const std::string& plain)
{
  static std::string translate = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  std::string result((plain.size()+2)/3*4, '=');
  uint i = 2;
  uint j = 0;
  while (i < plain.size())
  {
    result[j++] = translate[(plain[i-2]>>2) & 0x3f];
    result[j++] = translate[((plain[i-2]<<4) & 0x30) | ((plain[i-1]>>4) & 0xf)];
    result[j++] = translate[((plain[i-1]<<2) & 0x3c) | ((plain[i]>>6) & 0x3)];
    result[j++] = translate[plain[i] & 0x3f];
    i += 3;
  }
  if (i-1 < plain.size())
  {
    result[j++] = translate[(plain[i-2]>>2) & 0x3f];
    result[j++] = translate[((plain[i-2]<<4) & 0x30) | ((plain[i-1]>>4) & 0xf)];
    result[j++] = translate[((plain[i-1]<<2) & 0x3c)];
  }
  else if (i-2 < plain.size())
  {
    result[j++] = translate[(plain[i-2]>>2) & 0x3f];
    result[j++] = translate[((plain[i-2]<<4) & 0x30)];
  }

  return result;
}
