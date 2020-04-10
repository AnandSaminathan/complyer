#include <fstream>

#include "./nusmv/NuSMVListener.h"
#include "complyer/nusmv/NuSMV.hpp"
#include "interpreter/Interpreter.h"
#include "InputOptions.h"
#include "StringConstants.h"

using namespace antlr4;

NuSMV constructModel(InputOptions &inputOptions) {
  ANTLRInputStream input((std::istream &) inputOptions.getInputProgramStream());
  NuSMVLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  NuSMVParser parser(&tokens);

  tree::ParseTree *tree = parser.nusmv();
  NuSMVListener listener;
  tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

  return listener.getNuSMV();
}

void printVariables(NuSMV& nusmv) {
  std::cout << "Variables\n";
  auto symbols = nusmv.getSymbols();
  for(auto symbol: symbols) {
    std::cout  << symbol.getName() << " ";
  }
  std:: cout << '\n';
}

void printKripke(Kripke &spec){
  std::cout << "I: " << spec.getI() << '\n';
  std::cout << "T: " << spec.getT() << '\n';
}

void verifyPropertyInProgram(NuSMV &nusmv, Interpreter &interpreter){
  auto specifications = nusmv.getSpecifications();
  for(auto specification : specifications) {
    std::string property;
    if(specification.getType() == SAFETYSPEC) {
      property = StringConstants::SAFETYSPEC + " " + specification.getProperty();
      interpreter.interpret(property);
    } else {
      property = StringConstants::BOUND + " " + specification.getBound();
      interpreter.interpret(property);
      property = StringConstants::LTLSPEC + " " + specification.getProperty();
      interpreter.interpret(property);
    }
  }
}

void runInteractive(InputOptions &inputOptions, Interpreter &interpreter){
  while(inputOptions.isInteractive()){
    std::string input;
    std::cout << ">>> ";
    getline(std::cin,input);
    interpreter.interpret(input);
  }
}

int main(int argc, char* argv[]) {
  InputOptions inputOptions(argc,argv);
  NuSMV nusmv = constructModel(inputOptions);
  Kripke k = nusmv.toFormula();
  if(inputOptions.isVerbose()) {
    printVariables(nusmv);
    printKripke(k);
  }

  Interpreter interpreter(nusmv.getSymbols(), k, nusmv.getMapping());
  verifyPropertyInProgram(nusmv, interpreter);

  runInteractive(inputOptions, interpreter);

  return 0;
}
