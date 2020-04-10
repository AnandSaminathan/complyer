#pragma once

#include <utility>
#include <verification-algorithms/common/verifier.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <complyer/util/Kripke.hpp>
#include "CommandResponse.h"

class CommandBase {
  public:
    CommandBase(const std::vector<Symbol>& symbols,Kripke kripke,const std::map<std::string,std::string>& labelMapper)
    : ltl_bmc_verifier(symbols,kripke.getI(),kripke.getT()),
      k_induction_verifier(symbols,kripke.getI(),kripke.getT()),
      command_bound(ltl_bmc_verifier),
      command_length(*common_verifier),
      command_ltlspec(ltl_bmc_verifier,labelMapper),
      command_safetyspec(k_induction_verifier,labelMapper),
      command_trace(*common_verifier)
      {}
    void resetModel(const std::vector<Symbol>& symbols,Kripke kripke,const std::map<std::string,std::string>& labelMapper);

    CommandResponse perform(const std::string &line);
  private:
    ltlBmc ltl_bmc_verifier;
    kInduction k_induction_verifier;
    Verifier *common_verifier = nullptr;
    class CommandInterface {
      public:
        explicit CommandInterface(std::string op): operation(std::move(op)){}
        virtual bool parse(std::string line) = 0;
        virtual int perform() = 0;
        inline std::string getOperation() { return operation;}
        virtual inline std::string getMessage(){ return std::__cxx11::string(); };
      private:
        std::string operation;
    };
    class CommandBound : public CommandInterface {
      public:
        explicit CommandBound(ltlBmc &lB);
        bool parse(std::string line) override;
        int perform() override;
      private:
        ltlBmc *ltl_bmc_verifier;
        int bound;
    };
    class CommandLength : public CommandInterface {
      public:
        explicit CommandLength(Verifier &v);
        void setVerifier(Verifier *v);
        bool parse(std::string line) override;
        int perform() override;
      private:
          Verifier *verifier;
    };
    class CommandLtlspec : public CommandInterface {
      public:
        CommandLtlspec(ltlBmc &a,std::map<std::string, std::string> labelMapper);
        bool parse(std::string line) override;
        int perform() override;
      private:
        ltlBmc *ltl_bmc_verifier;
        std::map<std::string, std::string> label_mapper;
        std::string property;
    };
    class CommandQuit : public CommandInterface {
      public:
        CommandQuit();
        bool parse(std::string line) override;
        int perform() override;
    };
    class CommandSafetyspec : public CommandInterface {
      public:
        CommandSafetyspec(kInduction &a, std::map<std::string, std::string> labelMapper);
        bool parse(std::string line) override;
        int perform() override;
      private:
        kInduction *k_induction_verifier;
        std::map<std::string, std::string> label_mapper;
        std::string property;
    };
    class CommandTrace : public CommandInterface {
      public:
        explicit CommandTrace(Verifier &v);
        void setVerifier(Verifier *v);
        bool parse(std::string line) override;
        int perform() override;
        std::string getMessage() override;
    private:
        Verifier *verifier;
    };

    CommandBound command_bound;
    CommandLength command_length;
    CommandLtlspec command_ltlspec;
    CommandQuit command_quit;
    CommandSafetyspec command_safetyspec;
    CommandTrace command_trace;
};
