#include <iostream>
#include <sstream>
#include <algorithm>
#include "complyer/Interpreter.hpp"

void Interpreter::interpret(std::string input) {

  std::stringstream stream(input);
  std::string command;
  stream >> command;
  std::transform(command.begin(), command.end(), command.begin(), ::toupper);

  if(command == "QUIT") { quit(); } 
  else if(command == "BOUND") {
    int bound;
    if(stream >> bound) { ltlBmcVerifier.setBound(bound); } 
    else { std::cout << "Bound not specified\n"; }
  } else {
    std::string property = "";
    getline(stream, property);
    if(property == "") {
      std::cout << "Property not specified\n";
      return ;
    }

    if(command == "LTLSPEC") {
      bool result = ltlBmcVerifier.check(property);
      if(result) { std::cout << "model satisfies given property\n"; }
      else if(handleRejection()) { std::cout << ltlBmcVerifier.getTrace() << "\n"; }
    } else if(command == "SAFETYSPEC") {
      bool result = kInductionVerifier.check(property);
      if(result) { std::cout << "model satisfies given property\n"; }
      else if(handleRejection()) { std::cout << kInductionVerifier.getTrace() << "\n"; }
    } else {
      std::cout << "\nCommand '" << command << "' not found\n"; 
    }
  }
}

bool Interpreter::handleRejection() {
  std::cout << "model does not satisfy given property\n";
  std::cout << "print trace? ( y / n ) ";

  std::string option;
  getline(std::cin, option);
  return (option == "Y" or option == "y");
}