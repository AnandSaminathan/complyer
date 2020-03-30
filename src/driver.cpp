#include <fstream>

#include "NuSMVListener.h"
#include "complyer/ModelSpecification.h"
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
ModelSpecification constructModel(InputOptions &inputOptions) {
  ANTLRInputStream input(inputOptions.input_program_stream);
  NuSMVLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  NuSMVParser parser(&tokens);

  tree::ParseTree *tree = parser.module();
  NuSMVListener listener;
  tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

  return listener.getSpecification();
}
void printModelSpecification(InputOptions &inputOptions,ModelSpecification &spec){
  if(inputOptions.verbose) {
    std::vector<Symbol> symbol_set = spec.getSymbols();
    std::cout << "Variables: \n";
    for(Symbol s:symbol_set) {
      std::cout << s.getName() << ' ';
    }
    std::cout << '\n';
    std::cout << "I: " << spec.getI() << '\n';
    std::cout << "T: " << spec.getT() << '\n';
  }
}
void verifyPropertyInProgram(ModelSpecification &spec, Interpreter &interpreter){
  std::string property;
  if(spec.isLtl()){
    property = StringConstants::BOUND + " " + std::to_string(spec.getBound());
    interpreter.interpret(property);
    property = StringConstants::LTLSPEC + " " + spec.getP();
    interpreter.interpret(property);
  } else {
    property = StringConstants::SAFETYSPEC + " " + spec.getP();
    interpreter.interpret(property);
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
  ModelSpecification modelSpecification = constructModel(inputOptions);
  printModelSpecification(inputOptions, modelSpecification);

  Interpreter interpreter(modelSpecification.getSymbols(), modelSpecification.getI(),
          modelSpecification.getT(), modelSpecification.getLabelMapper());
  verifyPropertyInProgram(modelSpecification,interpreter);

  runInteractive(inputOptions, interpreter);

  return 0;
}
