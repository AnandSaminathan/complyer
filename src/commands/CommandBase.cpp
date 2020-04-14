#include "CommandBase.h"

#include <utility>
#include <NumericConstants.h>
#include <StringConstants.h>
#include <chrono>

CommandResponse CommandBase::perform(const std::string &line) {
  CommandInterface *command_interface = nullptr;
  if(command_quit.parse(line)) {
    command_interface = &command_quit;
  } else if(command_bound.parse(line)) {
    command_interface = &command_bound;
  } else if(command_length.parse(line)) {
    command_interface = &command_length;
  } else if(command_trace.parse(line)) {
    command_interface = &command_trace;
  } else if(command_ltlspec.parse(line)) {
    command_length.setVerifier(&ltl_bmc_verifier);
    command_trace.setVerifier(&ltl_bmc_verifier);
    command_interface = &command_ltlspec;
  } else if(command_safetyspec.parse(line)) {
    command_length.setVerifier(&k_induction_verifier);
    command_trace.setVerifier(&k_induction_verifier);
    command_interface = &command_safetyspec;
  } else {
    return CommandResponse(std::string(),0,"Invalid Command",0);
  }
  auto start = std::chrono::high_resolution_clock::now();
  int result = command_interface->perform();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return CommandResponse(command_interface->getOperation(),result,command_interface->getMessage(),duration.count());
}

CommandBase::CommandBound::CommandBound(ltlBmc &lB) : CommandInterface(StringConstants::BOUND) {
  ltl_bmc_verifier = &lB;
  bound = 100000;
}

bool CommandBase::CommandBound::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  if(command != getOperation()) return false;
  if(ss>>bound) return true;
  else throw std::invalid_argument("Bound is empty");
}

int CommandBase::CommandBound::perform() {
  ltl_bmc_verifier->setBound(bound);
  return int();
}

CommandBase::CommandLength::CommandLength(Verifier &v)  : CommandInterface(StringConstants::LENGTH) {
  verifier = &v;
}

void CommandBase::CommandLength::setVerifier(Verifier *v) {
  verifier = v;
}

bool CommandBase::CommandLength::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  return (command == getOperation());
}

int CommandBase::CommandLength::perform() {
  return verifier->getLength();
}

CommandBase::CommandLtlspec::CommandLtlspec(ltlBmc &a,std::map<std::string, std::string> labelMapper)
  :CommandInterface(StringConstants::LTLSPEC), label_mapper(std::move(labelMapper)) {
  ltl_bmc_verifier = &a;
}

bool CommandBase::CommandLtlspec::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  if(command != getOperation()) return false;
  std::getline(ss,property);
  if(property.empty()) throw std::invalid_argument("Property is empty");
  return true;
}

int CommandBase::CommandLtlspec::perform() {
  FormulaTree tree(property);
  tree.substitute(this->label_mapper);
  property = tree.getFormula();
  bool res = ltl_bmc_verifier->check(property);
  if(res) return NumericConstants::SAT;
  else return NumericConstants::UNSAT;
}

CommandBase::CommandQuit::CommandQuit():CommandInterface(StringConstants::QUIT) {}

bool CommandBase::CommandQuit::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  return (command == getOperation());
}

int CommandBase::CommandQuit::perform() {
  exit(0);
}

CommandBase::CommandSafetyspec::CommandSafetyspec(kInduction &a, std::map<std::string, std::string> labelMapper)
  : CommandInterface(StringConstants::SAFETYSPEC), label_mapper(std::move(labelMapper)) {
    k_induction_verifier = &a;
}

bool CommandBase::CommandSafetyspec::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  if(command != getOperation()) return false;
  std::getline(ss,property);
  if(property.empty()) throw std::invalid_argument("Property is empty");
  return true;
}

int CommandBase::CommandSafetyspec::perform() {
  FormulaTree tree(property);
  tree.substitute(this->label_mapper);
  property = tree.getFormula();
  bool res = k_induction_verifier->check(property);
  if(res) return NumericConstants::SAT;
  else return NumericConstants::UNSAT;
}

CommandBase::CommandTrace::CommandTrace(Verifier &v) : CommandInterface(StringConstants::TRACE) {
  verifier = &v;
}

void CommandBase::CommandTrace::setVerifier(Verifier *v) {
  verifier = v;
}

bool CommandBase::CommandTrace::parse(std::string line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;
  std::transform(command.begin(),command.end(),command.begin(),::toupper);
  return (command == getOperation());
}

int CommandBase::CommandTrace::perform() {
  return int();
}

std::string CommandBase::CommandTrace::getMessage() {
  std::stringstream ss;
  ss << verifier->getTrace();
  return ss.str();
}