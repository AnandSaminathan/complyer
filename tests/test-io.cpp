#include <iostream>
#include "complyer/util/io/io.hpp"

int main() {
  IO io;
  std::string str;
  while(true) {
    str = io.getline();
    std::cout << '[' << str << ']' << '\n';
  }
}
