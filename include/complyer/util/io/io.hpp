#pragma once

#include <string>
#include <vector>

std::vector<std::string> vocabulary = {
    "safetyspec", "ltlspec", "bound", "length", "trace", "quit", "ic3", "help", "pnet"};

#ifdef COMPLYER_USE_READLINE
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "readline/readline.h"
  #include "readline/history.h"


  char* commandNameGenerator(const char* text, int state) {
    static std::vector<std::string> matches;
    static size_t match_index = 0;

    if(state == 0) {
      matches.clear();
      match_index = 0;
      std::string textstr = std::string(text);

      for(auto word : vocabulary) {
        if (word.size() >= textstr.size() &&
            word.compare(0, textstr.size(), textstr) == 0) {
          matches.push_back(word);
        }
      }
    }

    if(match_index >= matches.size()) {
      return nullptr;
    } else {
      return strdup(matches[match_index++].c_str());
    }
  }

  char** commandCompletion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, commandNameGenerator);
  }
  
#endif

class IO {
  public:

    IO() = default;
    
    #ifdef COMPLYER_USE_READLINE
      static std::string getline() {
        rl_attempted_completion_function = commandCompletion;

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

