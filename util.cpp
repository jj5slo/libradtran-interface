#include<limits>

#include"util.h"

std::string itos(int digits, int val){	
  char buffer[std::numeric_limits<int>::digits10 + 1
          + 2]; // '-' + '\0'
  std::sprintf(buffer, "%d", val);
  return buffer;
}
