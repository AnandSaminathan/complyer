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
    else { std::cout << "\nBound not specified\n"; }
    std::cout << ltlBmcVerifier.getBound() << '\n';
  } else {
    std::string property = "";
    getline(stream, property);
    if(property == "") {
      std::cout << "\nProperty not specified\n";
      return ;
    }
    std::cout << "Checking: " << property << '\n';
    if(command == "LTLSPEC") {
      bool result = ltlBmcVerifier.check(property);
      if(result) { std::cout << "\nmodel satisfies given property\n"; }
      else if(handleRejection()) { std::cout << "\n\n" << ltlBmcVerifier.getTrace() << "\n"; }
    } else if(command == "SAFETYSPEC") {
      bool result = kInductionVerifier.check(property);
      if(result) { std::cout << "\nmodel satisfies given property\n"; }
      else if(handleRejection()) { std::cout << "\n\n" << kInductionVerifier.getTrace() << "\n"; }
    } else {
      std::cout << "\n\nCommand '" << command << "' not found\n\n"; 
    }
  }
}

bool Interpreter::handleRejection() {
  std::cout << "\nmodel does not satisfy given property\n\n";
  std::cout << "print trace? ( y / n ) ";

  std::string option;
  getline(std::cin, option);
  return (option == "Y" or option == "y");
}