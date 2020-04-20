#pragma once

#include <utility>
#include <verification-algorithms/common/verifier.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <complyer/util/Kripke.hpp>
#include "CommandResponse.h"

class CommandBase {
  public:
    struct CommandBaseSignature {
      std::vector<Symbol> symbols;
      Kripke kripke;
      std::map<std::string,std::string> label_mapper;
      CommandBaseSignature(std::vector<Symbol> s, Kripke k, std::map<std::string,std::string> labelMapper): symbols(std::move(s)), kripke(std::move(k)), label_mapper(std::move(labelMapper)){}
    };

    CommandBase(const std::vector<Symbol>& symbols,Kripke kripke,const std::map<std::string,std::string>& labelMapper);
    explicit CommandBase(const CommandBaseSignature& commandBaseSignature): CommandBase(commandBaseSignature.symbols, commandBaseSignature.kripke, commandBaseSignature.label_mapper){}
    CommandBase(const CommandBase &commandBase): CommandBase(commandBase.getSignature()) {};

    [[nodiscard]] CommandResponse perform(const std::string &line) const;
    [[nodiscard]] inline CommandBaseSignature getSignature() const {
      return signature;
    }

  private:
    class CommandInterface {
      public:
        CommandInterface(std::string op, std::shared_ptr<ltlBmc> lbv,
                         std::shared_ptr<kInduction> kiv);
        virtual void setVerifier(const std::shared_ptr<Verifier> &v){}
        inline std::string getOperation() { return operation;}
        virtual inline std::string getMessage(){ return std::__cxx11::string();};
        virtual bool parse(std::string line) = 0;
        virtual int perform() = 0;
      protected:
        std::shared_ptr<ltlBmc> ltl_bmc_verifier;
        std::shared_ptr<kInduction> k_induction_verifier;
        std::shared_ptr<Verifier> common_verifier;
      private:
        std::string operation;
    };
    class CommandBound : public CommandInterface {
      public:
        CommandBound(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv);
        bool parse(std::string line) override;
        int perform() override;
      private:
        int bound;
    };
    class CommandLength : public CommandInterface {
      public:
        CommandLength(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv);
        void setVerifier(const std::shared_ptr<Verifier> &v) override;
        bool parse(std::string line) override;
        int perform() override;
    };
    class CommandLtlspec : public CommandInterface {
      public:
        CommandLtlspec(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv, std::map<std::string, std::string> labelMapper);
        bool parse(std::string line) override;
        int perform() override;
      private:
        std::map<std::string, std::string> label_mapper;
        std::string property;
    };
    class CommandQuit : public CommandInterface {
      public:
        CommandQuit(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv);
        bool parse(std::string line) override;
        int perform() override;
    };
    class CommandSafetyspec : public CommandInterface {
      public:
        CommandSafetyspec(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv, std::map<std::string, std::string> labelMapper);
        bool parse(std::string line) override;
        int perform() override;
      private:
        std::map<std::string, std::string> label_mapper;
        std::string property;
    };
    class CommandTrace : public CommandInterface {
      public:
        CommandTrace(std::vector<std::shared_ptr<CommandInterface>> cmd, std::shared_ptr<ltlBmc> lbv, std::shared_ptr<kInduction> kiv);
        void setVerifier(const std::shared_ptr<Verifier> &v) override;
        bool parse(std::string line) override;
        int perform() override;
        std::string getMessage() override;
    };

    CommandBaseSignature signature;
    std::vector<std::shared_ptr<CommandInterface>> command;
    enum command_list{BOUND, LENGTH, LTLSPEC, QUIT, SAFETYSPEC, TRACE};

    std::shared_ptr<ltlBmc> ltl_bmc_verifier;
    std::shared_ptr<kInduction> k_induction_verifier;
};
