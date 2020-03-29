#include <chrono>
#include "Interpreter.h"

void Interpreter::interpret(const std::string& input) {
  if(quit.parse(input)){
    quit.perform();
  } else if(bound.parse(input)){
    std::string res = bound.perform();
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
    auto start = std::chrono::high_resolution_clock::now();
    std::string result = ltlspec.perform();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << result << std::endl;
    std::cout << "Time taken: " << duration.count()/1e6 << " seconds\n";
  } else if(safetyspec.parse(input)){
    common_verifier = &k_induction_verifier;
    trace.setVerifier(&k_induction_verifier);
    length.setVerifier(&k_induction_verifier);
    auto start = std::chrono::high_resolution_clock::now();
    std::string result = safetyspec.perform();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << result << std::endl;
    std::cout << "Time taken: " << duration.count()/1e6 << " seconds\n";
  } else {
    std::cout << "Invalid command\n";
  }
}
