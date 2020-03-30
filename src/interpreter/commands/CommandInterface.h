#pragma once

#include <string>
#include <utility>

class CommandInterface {

  std::string operation;

public:

  explicit CommandInterface(std::string operation_name){
    operation = std::move(operation_name);
  }
  virtual bool parse(std::string line) = 0;
  virtual std::string perform() = 0;
};
