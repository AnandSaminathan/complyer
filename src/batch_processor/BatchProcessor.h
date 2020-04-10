#pragma once

#include <commands/CommandBase.h>

class BatchProcessor {
  CommandBase command_base;
public:
  BatchProcessor(const std::vector<Symbol>& symbols, Kripke kripke, const std::map<std::string, std::string>& labelMapper) :
          command_base(symbols,std::move(kripke),labelMapper)
  {  }
  void process(const std::string &input);
};
