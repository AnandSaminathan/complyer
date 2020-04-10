//
// Created by User on 10-04-2020.
//

#include <StringConstants.h>
#include <NumericConstants.h>
#include "BatchProcessor.h"

void BatchProcessor::process(const std::string &input) {
  CommandResponse command_response = command_base.perform(input);
  if(command_response.getOperation() == StringConstants::LTLSPEC or command_response.getOperation() == StringConstants::SAFETYSPEC) {
    CommandResponse length_response = command_base.perform(StringConstants::LENGTH);
    if(command_response.getResult() == NumericConstants::SAT) {
      std::cout << "SAT";
      command_base.perform(StringConstants::BOUND + " " + std::to_string(length_response.getResult()));
    }
    else if(command_response.getResult() == NumericConstants::UNSAT) std::cout << "UNSAT";
    std::cout << ", " << command_response.getTimeTaken()/1e6 << ", ";
    std::cout << length_response.getResult() << "\n";
  }
}
