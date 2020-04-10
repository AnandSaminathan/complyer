//
// Created by User on 03-04-2020.
//

#ifndef SMV_INPUTOPTIONS_H
#define SMV_INPUTOPTIONS_H

#include <fstream>
#include <cassert>

class InputOptions {
public:
  InputOptions() {
    interactive = false;
    verbose = false;
    input_program_set = false;
    batch = false;
  }
  InputOptions(int argc, char* argv[]) : InputOptions() {
    parse(argc,argv);
  }
  void parse(int argc,char* argv[]){
    int it = 1;
    while(it < argc){
      std::string cur(argv[it]);
      if(cur == "-i" || cur == "--interactive"){
        this->interactive = true;
        it++;
      } else if(cur == "-v" || cur == "--verbose"){
        this->verbose = true;
        it++;
      } else if(cur == "-b" || cur == "--batch"){
        it++;
        if(it >= argc) throw std::invalid_argument("Batch input file not specified");
        this->batch_command_file = std::string(argv[it]);
        this->batch = true;
        it++;
      }
      else {
        this->input_program_set = true;
        this->input_program_stream.open(cur.c_str());
        it++;
      }
    }
    if(!this->input_program_set){
      throw std::invalid_argument("Program file not specified");
    }
    if(this->batch && this->interactive){
      throw std::invalid_argument("Interactive and Batch cannot be set simultaneously");
    }
  }

  bool isInteractive() const {
    return interactive;
  }

  bool isVerbose() const {
    return verbose;
  }

  bool isBatch() const {
    return batch;
  }

  std::string getBatchCommandFile() const {
    assert(batch);
    return batch_command_file;
  }

  const std::ifstream &getInputProgramStream() const {
    assert(input_program_set);
    return input_program_stream;
  }

private:
  bool interactive;
  bool verbose;
  bool batch;
  std::string batch_command_file;
  bool input_program_set;
  std::ifstream input_program_stream;
};
#endif //SMV_INPUTOPTIONS_H
