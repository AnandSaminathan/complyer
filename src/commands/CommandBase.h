#pragma once

#include <utility>
#include <memory>
#include <StringConstants.h>
#include <verification-algorithms/common/verifier.hpp>
#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <verification-algorithms/ic3/ic3.hpp>

#include "ModelSpecification.h"
#include "CommandResponse.h"

// TODO : Implement Destructors for proper memory deallocation
class CommandBase {
  public:
    explicit CommandBase(const ModelSpecification& m);
    CommandResponse perform(const std::string &line);
  private:
    class CommandInterface;
    class CommandQuit;
    class CommandTrace;
    class CommandLength;
    class CommandSafetyspec;
    class CommandBound;
    class CommandLtlspec;
    class CommandIC3;

    struct Commands;
    struct Verifiers;

    ModelSpecification model;
    Commands *command;
    Verifiers *verifier;
};

class CommandBase::CommandInterface {
  public:
    explicit CommandInterface(std::string op, Commands *commands, Verifiers *verifiers, ModelSpecification modelSpecification);
    [[nodiscard]] inline std::string getOperation() const { return operation; }
    inline virtual void prePerform(){};
    virtual bool parse(const std::string &);
    inline virtual void postPerform(){};
    virtual CommandResponse perform() = 0;
  protected:
    Commands *command;
    Verifiers *verifier;
    ModelSpecification model_specification;
  private:
    const std::string operation;
};

class CommandBase::CommandQuit : public CommandInterface {
  public:
    CommandQuit(Commands *cmds, Verifiers *vrfs, ModelSpecification ms) : CommandInterface(StringConstants::QUIT, cmds, vrfs, std::move(ms)){};
    CommandResponse perform() override;
};

class CommandBase::CommandTrace : public CommandInterface {
  public:
    CommandTrace(Commands *cmds, Verifiers *vrfs, ModelSpecification ms) : CommandInterface(StringConstants::TRACE, cmds, vrfs, std::move(ms)){};
    CommandResponse perform() override;
    void setVerifier(Verifier *current_verifier){ common_verifier = current_verifier; }
  private:
    Verifier *common_verifier{nullptr};
};

class CommandBase::CommandLength : public CommandInterface {
  public:
    CommandLength(Commands *cmds, Verifiers *vrfs, ModelSpecification ms) : CommandInterface(StringConstants::LENGTH, cmds, vrfs, std::move(ms)){};
    CommandResponse perform() override;
    void setVerifier(Verifier *current_verifier){ common_verifier = current_verifier; }
  private:
    Verifier *common_verifier{nullptr};
};

class CommandBase::CommandSafetyspec : public CommandInterface {
  public:
    CommandSafetyspec(Commands *cmds, Verifiers *vrfs, ModelSpecification ms): CommandInterface(StringConstants::SAFETYSPEC, cmds, vrfs, std::move(ms)){};
    bool parse(const std::string &) override;
    CommandResponse perform() override;
    void prePerform() override;
  private:
    std::string property;
};

class CommandBase::CommandBound : public CommandInterface {
  public:
    explicit CommandBound(Commands *cmds, Verifiers *vrfs, ModelSpecification ms): CommandInterface(StringConstants::BOUND, cmds, vrfs, std::move(ms)){};
    bool parse(const std::string &) override;
    CommandResponse perform() override;
  private:
    int bound{};
};

class CommandBase::CommandLtlspec : public CommandInterface {
  public:
    CommandLtlspec(Commands *cmds, Verifiers *vrfs, ModelSpecification ms): CommandInterface(StringConstants::LTLSPEC, cmds, vrfs, std::move(ms)){};
    bool parse(const std::string &) override;
    CommandResponse perform() override;
    void prePerform() override;
  private:
    std::string property;
};

class CommandBase::CommandIC3 : public CommandInterface {
  public:
    CommandIC3(Commands *cmds, Verifiers *vrfs, ModelSpecification ms) : CommandInterface(StringConstants::IC3, cmds, vrfs, std::move(ms)){};
    bool parse(const std::string &) override;
    CommandResponse perform() override;
    void prePerform() override;
  private:
    std::string property;
};

struct CommandBase::Verifiers {
  ltlBmc *ltl_bmc_verifier;
  kInduction *k_induction_verifier;
  IC3 *ic3_verifier;

  // VRFR_LIST_SIZE must always be the last element
  enum verifier_list{LTL_BMC, K_INDUCTION, IC_3, VRFR_LIST_SIZE};
  Verifier *verifiers[VRFR_LIST_SIZE]{};

  explicit Verifiers(ModelSpecification ms);
};

struct CommandBase::Commands {
  CommandQuit *quit;
  CommandTrace *trace;
  CommandLength *length;
  CommandSafetyspec *safetyspec;
  CommandBound *bound;
  CommandLtlspec *ltlspec;
  CommandIC3 *ic3;

  // CMD_LIST_SIZE must always be the last element
  enum command_list{QUIT, TRACE, LENGTH, SAFETYSPEC, BOUND, LTLSPEC, IC_3, CMD_LIST_SIZE};
  CommandInterface *commands[CMD_LIST_SIZE]{};

  Commands(Verifiers *verifiers, const ModelSpecification& ms);
};