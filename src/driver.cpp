#include <iostream>
#include <fstream>

#include "NuSMVListener.h"
#include "k-induction/k-induction.hpp"
#include "ltl-bmc/ltl-bmc.hpp"
#include "ModelSpecification.h"

using namespace antlr4;

int main(int argc, char* argv[]) {
  if(argc == 1) {
    std::cout << "Usage: " << argv[0] << " <file-name>\n";
    return 0;
  } 

  std::ifstream stream; stream.open(argv[1]);
        
  ANTLRInputStream input(stream);
  NuSMVLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  NuSMVParser parser(&tokens);

  tree::ParseTree *tree = parser.module();
  NuSMVListener listener;
  tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

  ModelSpecification spec = listener.getSpecification();

  auto x = spec.getSymbols();
  std::cout << "Variables: \n";
  for(auto y: x) {
    std::cout << y.getName() << ' ';
  }
  std::cout << '\n';
  std::cout << "I: " << spec.getI() << '\n';
  std::cout << "T: " << spec.getT() << '\n';

  if(!spec.isLtl()) {
    std::cout << "SAFETYSPEC\n"; 
    kInduction k(spec.getSymbols(), spec.getI(), spec.getT());
    do {
      std::cout << "Checking P: " << spec.getP() << '\n';
      if(k.check(spec.getP())) { std::cout << "Model satisfies the given property" << '\n'; }
      else { std::cout << "Model does not satisfy the given property" << '\n'; }

      std::cout << "Enter another property: ";
      std::string P; std::getline(std::cin, P);
      spec.setP(P);
    } while(true);
  } else {
    std::cout << "LTLSPEC " << " bound: " << spec.getBound() << "\n";
    ltlBmc l(spec.getSymbols(), spec.getI(), spec.getT()); l.setBound(spec.getBound());
    do {
      std::cout << "Checking P: " << spec.getP() << '\n';
      if(l.check(spec.getP())) { std::cout << "Model satisfies the given property" << '\n'; }
      else { std::cout << "Model does not satisfy the given property" << '\n'; }

      std::cout << "Enter another property: ";
      std::string P; std::getline(std::cin, P);
      spec.setP(P);
    } while(true);    
  }

  return 0;
}
