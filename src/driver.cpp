#include <iostream>
#include "NuSMVListener.h"
#include "k-induction/k-induction.hpp"

using namespace antlr4;

int main(int argc, char* argv[]) {
  if(argc == 1) {
    std::cout << "Usage: " << argv[0] << " <file-name>\n";
    return 0;
  } 
        
  std::ifstream stream;
  stream.open(argv[1]);

  ANTLRInputStream input(stream);
  NuSMVLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  NuSMVParser parser(&tokens);

  return 0;
}
