#include "CommandBase.h"

#include <utility>
#include <algorithm>
#include <chrono>
#include <NumericConstants.h>

CommandBase::CommandBase(ModelSpecification m): model(m) {
  verifier = std::make_shared<Verifiers>(Verifiers(m.getSymbols(), m.getKripke()));
  command = std::make_shared<Commands>(Commands(verifier, m.getLabelMapper()));
}

CommandResponse CommandBase::perform(const std::string &line) {
  if(command->quit->parse(line)) return command->quit->perform();
  if(command->trace->parse(line)) return command->trace->perform();
  if(command->length->parse(line)) return command->length->perform();
  if(command->bound->parse(line)) return command->bound->perform();
  if(command->safetyspec->parse(line)){
    command->length->setVerifier(verifier->k_induction_verifier);
    command->trace->setVerifier(verifier->k_induction_verifier);
    return command->safetyspec->perform();
  }
  if(command->ltlspec->parse(line)){
    command->length->setVerifier(verifier->ltl_bmc_verifier);
    command->trace->setVerifier(verifier->ltl_bmc_verifier);
    return command->ltlspec->perform();
  }
  if(command->ic3->parse(line)){
    command->length->setVerifier(verifier->ic3_verifier);
    command->trace->setVerifier(verifier->ic3_verifier);
    return command->ic3->perform();
  }
  return CommandResponse(std::string(), int(), "Invalid Command", long());
}

bool CommandBase::CommandInterface::parse(const std::string &line) {
  std::stringstream stream(line);
  std::string current_command;
  stream >> current_command;
  std::transform(current_command.begin(), current_command.end(), current_command.begin(), ::toupper);
  return current_command == this->getOperation();
}

CommandResponse CommandBase::CommandQuit::perform() {
  exit(0);
}

CommandResponse CommandBase::CommandTrace::perform() {
  assert(common_verifier != nullptr);
  try {
    auto start = std::chrono::high_resolution_clock::now();
    std::stringstream stream;
    auto trace = common_verifier->getTrace();
    auto states = trace.getStates();
    auto symbols = trace.getSymbols();
    for(int j=0; j<states.size();j++) {
      auto state = states[j];
      int vars = symbols.size();
      stream << "State " << j+1 << '\n';
      for(int i = 0; i < vars; ++i) {
        stream << "\t" << symbols[i] << " = " << state[i] << "\n";
      }
      stream << '\n';
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    return CommandResponse(this->getOperation(), 1, stream.str(), duration.count());
  } catch (std::exception &e) {
    return CommandResponse(this->getOperation(), 0, e.what(), 0);
  }
}

void CommandBase::CommandTrace::setVerifier(std::shared_ptr<Verifier> v) {
  common_verifier = std::move(v);
}

CommandResponse CommandBase::CommandLength::perform() {
  assert(common_verifier != nullptr);
  auto start = std::chrono::high_resolution_clock::now();
  int result = common_verifier->getLength();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return CommandResponse(this->getOperation(), result, std::to_string(result), duration.count());
}

void CommandBase::CommandLength::setVerifier(std::shared_ptr<Verifier> v) {
  common_verifier = std::move(v);
}

bool CommandBase::CommandSafetyspec::parse(const std::string &line) {
  if(!CommandInterface::parse(line)) return false;
  std::stringstream stream(line);
  std::string cmd; stream >> cmd;
  std::getline(stream, this->property);
  return true;
}

CommandResponse CommandBase::CommandSafetyspec::perform() {
  auto start = std::chrono::high_resolution_clock::now();
  FormulaTree tree(this->property);
  tree.substitute(this->label_mapper);
  this->property = tree.getFormula();
  int result = k_induction_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}

bool CommandBase::CommandBound::parse(const std::string &line) {
  if(!CommandInterface::parse(line)) return false;
  std::stringstream stream(line);
  std::string cmd; stream >> cmd;
  stream >> bound;
  return true;
}

CommandResponse CommandBase::CommandBound::perform() {
  auto start = std::chrono::high_resolution_clock::now();
  ltl_bmc_verifier->setBound(bound);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return CommandResponse(getOperation(), bound, "Set " + std::to_string(bound), duration.count());
}

bool CommandBase::CommandLtlspec::parse(const std::string &line) {
  if(!CommandInterface::parse(line)) return false;
  std::stringstream stream(line);
  std::string cmd; stream >> cmd;
  std::getline(stream, property);
  return true;
}

CommandResponse CommandBase::CommandLtlspec::perform() {
  auto start = std::chrono::high_resolution_clock::now();
  FormulaTree tree(this->property);
  tree.substitute(this->label_mapper);
  this->property = tree.getFormula();
  int result = ltl_bmc_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}

CommandBase::Commands::Commands(const std::shared_ptr<Verifiers> &verifier, const std::map<std::string, std::string> &labelMapper) {
  quit = std::make_shared<CommandQuit>(CommandQuit());
  trace = std::make_shared<CommandTrace>(CommandTrace());
  length = std::make_shared<CommandLength>(CommandLength());
  safetyspec = std::make_shared<CommandSafetyspec>(CommandSafetyspec(verifier->k_induction_verifier, labelMapper));
  bound = std::make_shared<CommandBound>(CommandBound(verifier->ltl_bmc_verifier));
  ltlspec = std::make_shared<CommandLtlspec>(CommandLtlspec(verifier->ltl_bmc_verifier, labelMapper));
  ic3 = std::make_shared<CommandIC3>(CommandIC3(verifier->ic3_verifier, labelMapper));
}

CommandBase::Verifiers::Verifiers(const std::vector<Symbol> &symbols, Kripke kripke) {
  ltl_bmc_verifier = std::make_shared<ltlBmc>(ltlBmc(symbols, kripke.getI(), kripke.getT()));
  k_induction_verifier = std::make_shared<kInduction>(kInduction(symbols, kripke.getI(), kripke.getT()));
  ic3_verifier = std::make_shared<IC3>(IC3(symbols, kripke.getI(), kripke.getT()));
}

bool CommandBase::CommandIC3::parse(const std::string &line) {
  if(!CommandInterface::parse(line)) return false;
  std::stringstream stream(line);
  std::string cmd; stream >> cmd;
  std::getline(stream, this->property);
  return true;
}

CommandResponse CommandBase::CommandIC3::perform() {
  auto start = std::chrono::high_resolution_clock::now();
  FormulaTree tree(this->property);
  tree.substitute(this->label_mapper);
  this->property = tree.getFormula();
  int result = ic3_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}
