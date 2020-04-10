#include <NumericConstants.h>
#include <StringConstants.h>
#include "Interpreter.h"

void Interpreter::interpret(const std::string& input) {
  CommandResponse command_response = command_base.perform(input);
  if(command_response.getOperation() == StringConstants::QUIT or command_response.getOperation() == StringConstants::BOUND) {}
  else if(command_response.getOperation() == StringConstants::SAFETYSPEC or command_response.getOperation() == StringConstants::LTLSPEC) {
    if(command_response.getResult() == NumericConstants::SAT) {
      std::cout << "The models satisfies the given property\nTime taken: " << command_response.getTimeTaken()/1e6 << std::endl;
      CommandResponse model_length = command_base.perform(StringConstants::LENGTH);
      model_length = command_base.perform(StringConstants::BOUND + " " + std::to_string(model_length.getResult()));
    } else if(command_response.getResult() == NumericConstants::UNSAT) {
      std::cout << "The models does not satisfy the given property\nTime taken: " << command_response.getTimeTaken()/1e6 << std::endl;
    }
  }
  else if(command_response.getOperation() == StringConstants::LENGTH) {
    std::cout << command_response.getResult() << std::endl;
  }
  else {
    std::cout << command_response.getMessage() << std::endl;
  }
}
