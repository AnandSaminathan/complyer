#pragma once

#include <string>

#ifdef COMPLYER_USE_READLINE
  #include "readline/readline.h"
  #include "readline/history.h"
#endif

class IO {
  public:

    IO() = default;
    
    #ifdef COMPLYER_USE_READLINE
      static std::string getline() {
        const char* line = readline(">>> ");
        if(line == nullptr) { exit(0); }
        if(*line) add_history(line);
        std::string ret(line);
        free( (void*) line);
        return ret;
      }
    #else
      static std::string getline() {
        std::string line;
        std::cout << ">>> ";
        std::getline(std::cin, line);
        if(std::cin.eof()) { exit(0); }
        return line;
      }
    #endif
};

