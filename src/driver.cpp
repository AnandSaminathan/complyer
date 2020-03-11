#include <fstream>

#include "NuSMVListener.h"
#include "complyer/ModelSpecification.h"
#include "complyer/Interpreter.hpp"

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

  Interpreter interpreter(spec.getSymbols(), spec.getI(), spec.getT());
  std::string property;

  if(spec.isLtl()) {
    std::string bound = "BOUND " + std::to_string(spec.getBound());
    interpreter.interpret(bound);
    property = "LTLSPEC " + spec.getP();
  } else {
    property = "SAFETYSPEC " + spec.getP();
  }

  interpreter.interpret(property);

  while(true) {
    std::string input;
    std::cout << ">>> ";
    getline(std::cin, input);
    interpreter.interpret(input);
  }

  return 0;
}
