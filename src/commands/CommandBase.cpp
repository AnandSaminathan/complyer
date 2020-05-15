#include "CommandBase.h"

#include <utility>
#include <algorithm>
#include <chrono>
#include <NumericConstants.h>

CommandBase::CommandBase(const ModelSpecification& m): model(m) {
  verifier = new Verifiers(m);
  command = new Commands(verifier, m);
}

CommandResponse CommandBase::perform(const std::string &line) {
  for(auto & cur_command : command->commands){
    if(cur_command->parse(line)){
      cur_command->prePerform();
      CommandResponse res = cur_command->perform();
      cur_command->postPerform();
      return res;
    }
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

CommandBase::CommandInterface::CommandInterface(std::string op, CommandBase::Commands *commands,
                                                CommandBase::Verifiers *verifiers,
                                                ModelSpecification modelSpecification) : operation(std::move(op)) {
  this->command = commands;
  this->verifier = verifiers;
  this->model_specification = std::move(modelSpecification);
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

CommandResponse CommandBase::CommandLength::perform() {
  assert(common_verifier != nullptr);
  auto start = std::chrono::high_resolution_clock::now();
  int result = common_verifier->getLength();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return CommandResponse(this->getOperation(), result, std::to_string(result), duration.count());
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
  auto label_mapper = this->model_specification.getLabelMapper();
  tree.substitute(label_mapper);
  this->property = tree.getFormula();
  int result = this->verifier->k_induction_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}

void CommandBase::CommandSafetyspec::prePerform() {
  command->trace->setVerifier(verifier->k_induction_verifier);
  command->length->setVerifier(verifier->k_induction_verifier);
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
  this->verifier->ltl_bmc_verifier->setBound(bound);
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
  auto label_mapper = this->model_specification.getLabelMapper();
  tree.substitute(label_mapper);
  this->property = tree.getFormula();
  int result = this->verifier->ltl_bmc_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}

void CommandBase::CommandLtlspec::prePerform() {
  command->trace->setVerifier(verifier->ltl_bmc_verifier);
  command->length->setVerifier(verifier->ltl_bmc_verifier);
}

CommandBase::Commands::Commands(Verifiers *verifiers, const ModelSpecification& ms) {
  commands[IC_3] = ic3 = new CommandIC3(this, verifiers, ms);
  commands[QUIT] = quit = new CommandQuit(this, verifiers, ms);
  commands[TRACE] = trace = new CommandTrace(this, verifiers, ms);
  commands[BOUND] = bound = new CommandBound(this, verifiers, ms);
  commands[LENGTH] = length = new CommandLength(this, verifiers, ms);
  commands[LTLSPEC] = ltlspec = new CommandLtlspec(this, verifiers, ms);
  commands[SAFETYSPEC] = safetyspec = new CommandSafetyspec(this, verifiers, ms);
}

CommandBase::Verifiers::Verifiers(ModelSpecification ms) {
  verifiers[LTL_BMC] = ltl_bmc_verifier = new ltlBmc(ms.getSymbols(), ms.getKripke().getI(), ms.getKripke().getT());
  verifiers[K_INDUCTION] = k_induction_verifier = new kInduction(ms.getSymbols(), ms.getKripke().getI(), ms.getKripke().getT());
  verifiers[IC_3] = ic3_verifier = new IC3(ms.getSymbols(), ms.getKripke().getI(), ms.getKripke().getT());
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
  auto label_mapper = this->model_specification.getLabelMapper();
  tree.substitute(label_mapper);
  this->property = tree.getFormula();
  int result = this->verifier->ic3_verifier->check(property)?NumericConstants::SAT : NumericConstants::UNSAT;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::string result_string = result == NumericConstants::SAT ? "SAT" : "UNSAT";
  return CommandResponse(getOperation(), result, result_string, duration.count());
}

void CommandBase::CommandIC3::prePerform() {
  command->trace->setVerifier(verifier->ic3_verifier);
  command->length->setVerifier(verifier->ic3_verifier);
}
