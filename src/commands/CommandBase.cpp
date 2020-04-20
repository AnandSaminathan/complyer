#include "CommandBase.h"

#include <utility>
#include <NumericConstants.h>
#include <StringConstants.h>
#include <chrono>

CommandBase::CommandBase(const std::vector<Symbol>& symbols,Kripke kripke,const std::map<std::string,std::string>& labelMapper): signature(symbols, std::move(kripke), labelMapper) {
  ltl_bmc_verifier = std::make_shared<ltlBmc>(ltlBmc(signature.symbols, signature.kripke.getI(), signature.kripke.getT()));
  k_induction_verifier = std::make_shared<kInduction>(kInduction(signature.symbols, signature.kripke.getI(), signature.kripke.getT()));
  command = std::vector<std::shared_ptr<CommandInterface>>(6);

  command[BOUND] = std::make_shared<CommandBound>(CommandBound(command, ltl_bmc_verifier, k_induction_verifier));
  command[LENGTH] = std::make_shared<CommandLength>(CommandLength(command, ltl_bmc_verifier, k_induction_verifier));
  command[LTLSPEC] = std::make_shared<CommandLtlspec>(CommandLtlspec(command, ltl_bmc_verifier, k_induction_verifier, signature.label_mapper));
  command[SAFETYSPEC] = std::make_shared<CommandSafetyspec>(CommandSafetyspec(command, ltl_bmc_verifier, k_induction_verifier, signature.label_mapper));
  command[TRACE] = std::make_shared<CommandTrace>(CommandTrace(command, ltl_bmc_verifier, k_induction_verifier));
  command[QUIT] = std::make_shared<CommandQuit>(CommandQuit(command, ltl_bmc_verifier, k_induction_verifier));
}

CommandResponse CommandBase::perform(const std::string &line) const {
  std::shared_ptr<CommandInterface> current_command;
  if(command[QUIT]->parse(line)) {
    current_command = command[QUIT];
  } else if(command[BOUND]->parse(line)) {
    current_command = command[BOUND];
  } else if(command[LENGTH]->parse(line)) {
    current_command = command[LENGTH];
  } else if(command[TRACE]->parse(line)) {
    current_command = command[TRACE];
  } else if(command[LTLSPEC]->parse(line)) {
    command[LENGTH]->setVerifier(ltl_bmc_verifier);
    command[TRACE]->setVerifier(ltl_bmc_verifier);
    current_command = command[LTLSPEC];
  } else if(command[SAFETYSPEC]->parse(line)) {
    command[LENGTH]->setVerifier(k_induction_verifier);
    command[TRACE]->setVerifier(k_induction_verifier);
    current_command = command[SAFETYSPEC];
  } else {
    return CommandResponse(std::string(),0,"Invalid Command",0);
  }
  auto start = std::chrono::high_resolution_clock::now();
  int result = current_command->perform();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  return CommandResponse(current_command->getOperation(),result,current_command->getMessage(),duration.count());
}

CommandBase::CommandInterface::CommandInterface(std::string op, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv)
        : operation(std::move(op)), ltl_bmc_verifier(std::move(lbv)), k_induction_verifier(std::move(kiv)){}

CommandBase::CommandBound::CommandBound(std::vector<std::shared_ptr<CommandInterface>> cmd,
        std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv)
        : CommandInterface(StringConstants::BOUND, std::move(lbv), std::move(kiv)) {
  bound = 100000;
}

bool CommandBase::CommandBound::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  if(command_prompt != getOperation()) return false;
  if(ss >> bound) return true;
  else throw std::invalid_argument("Bound is empty");
}

int CommandBase::CommandBound::perform() {
  this->ltl_bmc_verifier->setBound(bound);
  return int();
}

CommandBase::CommandLength::CommandLength(std::vector<std::shared_ptr<CommandInterface>> cmd,
        std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv)
        : CommandInterface(StringConstants::LENGTH, std::move(lbv), std::move(kiv)) {
  common_verifier = nullptr;
}

void CommandBase::CommandLength::setVerifier(const std::shared_ptr<Verifier> &v) {
  common_verifier = v;
}

bool CommandBase::CommandLength::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  return (command_prompt == getOperation());
}

int CommandBase::CommandLength::perform() {
  assert(common_verifier != nullptr);
  return common_verifier->getLength();
}

CommandBase::CommandLtlspec::CommandLtlspec(std::vector<std::shared_ptr<CommandInterface>> cmd,
        std::shared_ptr<ltlBmc> lbv,
        std::shared_ptr<kInduction> kiv,
        std::map<std::string, std::string> labelMapper)
  : CommandInterface(StringConstants::LTLSPEC, lbv, kiv), label_mapper(std::move(labelMapper)) {}

bool CommandBase::CommandLtlspec::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  if(command_prompt != getOperation()) return false;
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

CommandBase::CommandQuit::CommandQuit(std::vector<std::shared_ptr<CommandInterface>> cmd,
        std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv)
        : CommandInterface(StringConstants::QUIT, lbv, kiv) {}

bool CommandBase::CommandQuit::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  return (command_prompt == getOperation());
}

int CommandBase::CommandQuit::perform() {
  exit(0);
}

CommandBase::CommandSafetyspec::CommandSafetyspec(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv,
        std::shared_ptr<kInduction> kiv, std::map<std::string, std::string> labelMapper)
        : CommandInterface(StringConstants::SAFETYSPEC, lbv, kiv), label_mapper(std::move(labelMapper)) {}

bool CommandBase::CommandSafetyspec::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  if(command_prompt != getOperation()) return false;
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

CommandBase::CommandTrace::CommandTrace(std::vector<std::shared_ptr<CommandInterface>> cmd,
        std::shared_ptr<ltlBmc> lbv,
        std::shared_ptr<kInduction> kiv)
        : CommandInterface(StringConstants::TRACE, lbv, kiv) {
  common_verifier = nullptr;
}

void CommandBase::CommandTrace::setVerifier(const std::shared_ptr<Verifier> &v) {
  common_verifier = v;
}

bool CommandBase::CommandTrace::parse(std::string line) {
  std::stringstream ss(line);
  std::string command_prompt;
  ss >> command_prompt;
  std::transform(command_prompt.begin(), command_prompt.end(), command_prompt.begin(), ::toupper);
  return (command_prompt == getOperation());
}

int CommandBase::CommandTrace::perform() {
  return int();
}

std::string CommandBase::CommandTrace::getMessage() {
  std::stringstream ss;
  ss << common_verifier->getTrace();
  return ss.str();
}