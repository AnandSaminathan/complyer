#include "Interpreter.h"

void Interpreter::interpret(const std::string& input) {
  if(quit.parse(input)){
    quit.perform();
  } else if(bound.parse(input)){
    std::string res = bound.perform();
//        std::cout << res << std::endl;
  } else if(length.parse(input)) {
    std::string res = length.perform();
    std::cout << res << std::endl;
  } else if(trace.parse(input)) {
    std::string res = trace.perform();
    std::cout << res << std::endl;
  } else if(ltlspec.parse(input)){
    common_verifier = &ltl_bmc_verifier;
    trace.setVerifier(&ltl_bmc_verifier);
    length.setVerifier(&ltl_bmc_verifier);
    std::string result = ltlspec.perform();
    std::cout << result << std::endl;
  } else if(safetyspec.parse(input)){
    common_verifier = &k_induction_verifier;
    trace.setVerifier(&k_induction_verifier);
    length.setVerifier(&k_induction_verifier);
    std::string result = safetyspec.perform();
    std::cout << result << std::endl;
  } else {
    std::cout << "Invalid command\n";
  }
}
