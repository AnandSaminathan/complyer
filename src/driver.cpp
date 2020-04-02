#include <fstream>

#include "./nusmv/NuSMVListener.h"
#include "complyer/nusmv/NuSMV.hpp"
#include "interpreter/Interpreter.h"

using namespace antlr4;

struct InputOptions {
  bool interactive;
  bool verbose;
  bool input_program_set;
  std::ifstream input_program_stream;
  InputOptions(){
      interactive = false;
      verbose = false;
      input_program_set = false;
  }
};

InputOptions processInput(int argc,char* argv[]){
  int it = 1;
  InputOptions inputOptions;
  while(it < argc){
    std::string cur(argv[it]);
    if(cur == "-i" || cur == "--interactive"){
      inputOptions.interactive = true;
      it++;
    } else if(cur == "-v" || cur == "--verbose"){
      inputOptions.verbose = true;
      it++;
    } else {
      inputOptions.input_program_set = true;
      inputOptions.input_program_stream.open(cur.c_str());
      it++;
    }
  }
  if(!inputOptions.input_program_set){
    throw std::invalid_argument("Program file not specified");
  }
  return inputOptions;
}

NuSMV constructModel(InputOptions &inputOptions) {
  ANTLRInputStream input(inputOptions.input_program_stream);
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
  while(inputOptions.interactive){
    std::string input;
    std::cout << ">>> ";
    getline(std::cin,input);
    interpreter.interpret(input);
  }
}

int main(int argc, char* argv[]) {
  InputOptions inputOptions = processInput(argc,argv);
  NuSMV nusmv = constructModel(inputOptions);
  Kripke k = nusmv.toFormula();
  if(inputOptions.verbose) {
    printVariables(nusmv);
    printKripke(k);
  }

  Interpreter interpreter(nusmv.getSymbols(), k.getI(),
          k.getT(), nusmv.getMapping());
  verifyPropertyInProgram(nusmv, interpreter);

  runInteractive(inputOptions, interpreter);

  return 0;
}
