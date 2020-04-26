#pragma once

#include <utility>
#include <memory>
#include <StringConstants.h>
#include <verification-algorithms/common/verifier.hpp>
#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>

#include "ModelSpecification.h"
#include "CommandResponse.h"

class CommandBase {
  public:
    explicit CommandBase(ModelSpecification m);
    CommandResponse perform(const std::string &line);
  private:
    class CommandInterface;
    class CommandQuit;
    class CommandTrace;
    class CommandLength;
    class CommandSafetyspec;
    class CommandBound;
    class CommandLtlspec;

    struct Commands;
    struct Verifiers;

    ModelSpecification model;
    std::shared_ptr<Commands> command;
    std::shared_ptr<Verifiers> verifier;
};

class CommandBase::CommandInterface {
  public:
    explicit CommandInterface(std::string op): operation(std::move(op)){};
    [[nodiscard]] inline std::string getOperation() const { return operation; }
    virtual bool parse(const std::string &);
    virtual CommandResponse perform() = 0;
  private:
    const std::string operation;
};

class CommandBase::CommandQuit : public CommandInterface {
  public:
    CommandQuit() : CommandInterface(StringConstants::QUIT){};
    CommandResponse perform() override;
};

class CommandBase::CommandTrace : public CommandInterface {
  public:
    CommandTrace() : CommandInterface(StringConstants::TRACE){ (this->common_verifier) = nullptr; };
    CommandResponse perform() override;
    void setVerifier(std::shared_ptr<Verifier>);
  private:
    std::shared_ptr<Verifier> common_verifier;
};

class CommandBase::CommandLength : public CommandInterface {
  public:
    CommandLength() : CommandInterface(StringConstants::LENGTH){ (this->common_verifier) = nullptr; };
    CommandResponse perform() override;
    void setVerifier(std::shared_ptr<Verifier>);
  private:
    std::shared_ptr<Verifier> common_verifier;
};

class CommandBase::CommandSafetyspec : public CommandInterface {
  public:
    CommandSafetyspec(std::shared_ptr<kInduction> kiv, std::map<std::string, std::string> lm): CommandInterface(StringConstants::SAFETYSPEC), k_induction_verifier(std::move(kiv)), label_mapper(std::move(lm)){}
    bool parse(const std::string &) override;
    CommandResponse perform() override;
  private:
    std::shared_ptr<kInduction> k_induction_verifier;
    std::map<std::string, std::string> label_mapper;
    std::string property;
};

class CommandBase::CommandBound : public CommandInterface {
  public:
    explicit CommandBound(std::shared_ptr<ltlBmc> lbv): CommandInterface(StringConstants::BOUND), ltl_bmc_verifier(std::move(lbv)){}
    bool parse(const std::string &) override;
    CommandResponse perform() override;
  private:
    std::shared_ptr<ltlBmc> ltl_bmc_verifier;
    int bound{};
};

class CommandBase::CommandLtlspec : public CommandInterface {
  public:
    CommandLtlspec(std::shared_ptr<ltlBmc> lbv, std::map<std::string, std::string> lm): CommandInterface(StringConstants::LTLSPEC), ltl_bmc_verifier(std::move(lbv)), label_mapper(std::move(lm)){}
    bool parse(const std::string &) override;
    CommandResponse perform() override;
  private:
    std::shared_ptr<ltlBmc> ltl_bmc_verifier;
    std::map<std::string, std::string> label_mapper;
    std::string property;
};

struct CommandBase::Verifiers {
  std::shared_ptr<ltlBmc> ltl_bmc_verifier;
  std::shared_ptr<kInduction> k_induction_verifier;
  Verifiers(const std::vector<Symbol>& symbols, Kripke kripke);
};

struct CommandBase::Commands {
  std::shared_ptr<CommandQuit> quit;
  std::shared_ptr<CommandTrace> trace;
  std::shared_ptr<CommandLength> length;
  std::shared_ptr<CommandSafetyspec> safetyspec;
  std::shared_ptr<CommandBound> bound;
  std::shared_ptr<CommandLtlspec> ltlspec;
  Commands(const std::shared_ptr<Verifiers> &verifier, const std::map<std::string, std::string>& labelMapper);
};