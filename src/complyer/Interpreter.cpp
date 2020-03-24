#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "Interpreter.hpp"
#include "formula-tree/formula-tree.h"

void Interpreter::interpret(std::string input) {

  std::stringstream stream(input);
  std::string command;
  stream >> command;
  std::transform(command.begin(), command.end(), command.begin(), ::toupper);

  if(command == "QUIT") { quit(); } 
  else if(command == "LENGTH") { 
    if(safetyProp) { std::cout << kInductionVerifier.getLength() << '\n'; } 
    else { std::cout << ltlBmcVerifier.getLength() << '\n'; }
  }
  else if(command == "BOUND") {
    int bound;
    if(stream >> bound) { ltlBmcVerifier.setBound(bound); } 
    else { std::cout << "Bound not specified\n"; }
  } else if(command == "LTLSPEC" || command == "SAFETYSPEC") {
    std::string property = "";
    getline(stream, property);
    if(property == "") {
      std::cout << "Property not specified\n";
      return ;
    }

    FormulaTree tree(property);
    tree.substitute(this->labelMapper);
    property = tree.getFormula();

    if(command == "LTLSPEC") {
      safetyProp = false;
      auto start = std::chrono::high_resolution_clock::now();
      bool result = ltlBmcVerifier.check(property);
      auto stop = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      std::cout << "Time taken: " << duration.count() / 1e6 << " seconds\n";
      if(result) { std::cout << "model satisfies given property\n"; }
      else if(handleRejection()) { std::cout << ltlBmcVerifier.getTrace() << "\n"; }
    } else if(command == "SAFETYSPEC") {
      safetyProp = true;
      auto start = std::chrono::high_resolution_clock::now();
      bool result = kInductionVerifier.check(property);
      auto stop = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      std::cout << "Time taken: " << duration.count() / 1e6 << " seconds\n";
      if(result) { std::cout << "model satisfies given property\n"; ltlBmcVerifier.setBound(kInductionVerifier.getLength()); }
      else if(handleRejection()) { std::cout << kInductionVerifier.getTrace() << "\n"; }
    } 
  } else {
    std::cout << "\nCommand '" << input << "' not found\n\n"; 
  }
}

bool Interpreter::handleRejection() {
  std::cout << "model does not satisfy given property\n";
  std::cout << "print trace? ( y / n ) ";

  std::string option;
  getline(std::cin, option);
  return (option == "Y" or option == "y");
}
