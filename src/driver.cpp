#include <fstream>
#include <interpreter/PrinterFunctions.h>
#include "./nusmv/NuSMVListener.h"
#include "interpreter/Interpreter.h"
#include "InputOptions.h"

#ifdef COMPLYER_USE_READLINE
  #include "complyer/util/io/io.hpp"
#endif


using namespace antlr4;

class Main {
  public:
    explicit Main(InputOptions &input_options) {
      inputOptions = &input_options;
    }
    void main() {
      NuSMV nusmv = constructModel();
      Kripke k = nusmv.toFormula();
      if(inputOptions->isVerbose()) {
        printVariables(nusmv);
        printKripke(k);
      }
      CommandBase commandBase(ModelSpecification(nusmv.getSymbols(), k, nusmv.getMapping()));
      Interpreter interpreter(commandBase);
      if(inputOptions->isBatch()) interpreter.setPrinter(PrinterFunctions::batch);
      else interpreter.setPrinter(PrinterFunctions::interactive);
      verifyPropertyInProgram(nusmv,interpreter);
      runInteractive(interpreter);
      runBatch(interpreter);
    }
  private:
    InputOptions *inputOptions;
    NuSMV constructModel() {
      ANTLRInputStream input((std::istream &) inputOptions->getInputProgramStream());
      NuSMVLexer lexer(&input);
      CommonTokenStream tokens(&lexer);
      NuSMVParser parser(&tokens);

      tree::ParseTree *tree = parser.nusmv();
      NuSMVListener listener;
      tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

      return listener.getNuSMV();
    }
    static void printVariables(NuSMV& nusmv) {
      std::cout << "Variables\n";
      auto symbols = nusmv.getSymbols();
      for(auto symbol: symbols) {
        std::cout  << symbol.getName() << " ";
      }
      std:: cout << '\n';
    }
    static void printKripke(Kripke &spec){
      std::cout << "I: " << spec.getI() << '\n';
      std::cout << "T: " << spec.getT() << '\n';
    }
    static void verifyPropertyInProgram(NuSMV &nusmv, Interpreter &interpreter){
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
    void runInteractive(Interpreter &interpreter){
      #ifdef COMPLYER_USE_READLINE
        std::cout << "using readline\n";
        IO io;
        while(inputOptions->isInteractive()){
          std::string input = io.getline();
          interpreter.interpret(input);
        }
      #else
        std::cout << "not using readline\n";
        while(inputOptions->isInteractive()){
          std::string input;
          std::cout << ">>> ";
          getline(std::cin,input);
          interpreter.interpret(input);
        }
      #endif
    }
    void runBatch(Interpreter &interpreter){
      if(!inputOptions->isBatch()) return;
      std::ifstream input_stream(inputOptions->getBatchCommandFile().c_str());
      for(std::string line;std::getline(input_stream,line);) {
        interpreter.interpret(line);
      }
    }
};

int main(int argc, char* argv[]) {
  InputOptions inputOptions(argc,argv);
  Main main(inputOptions);
  main.main();
  return 0;
}
