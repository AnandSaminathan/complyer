#pragma once

#include <string>

#include "readline/readline.h"
#include "readline/history.h"

class IO {
  public:

    IO() {  }
    
    #ifdef COMPLYER_USE_READLINE
      std::string getline() {
        const char* line = readline(">>> ");
        if(line == nullptr) { exit(0); }
        if(*line) add_history(line);
        std::string ret(line);
        free( (void*) line);
        return ret;
      }
    #else
      std::string getline() {
        std::string line;
        std::cout << ">>> ";
        std::getline(std::cin, line);
        if(std::cin.eof()) { exit(0); }
        return line;
      }
    #endif
};

