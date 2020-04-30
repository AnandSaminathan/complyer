#pragma once

#include <string>

#include "readline/readline.h"
#include "readline/history.h"

class IO {
  public:

    IO() {  }
    
    std::string getline() {
      const char* line = readline(">>> ");
      if(line == nullptr) { exit(0); }
      if(*line) add_history(line);
      std::string ret(line);
      free( (void*) line);
      return ret;
    } 
};

