#pragma once

#include <string>
#include <utility>

class CommandResponse {
  public:
    CommandResponse(std::string operation, int result, std::string message, long timeTaken) : operation(std::move(operation)),result(result),message(std::move(message)),time_taken(timeTaken) {}
    [[nodiscard]] std::string getOperation() const {
      return operation;
    }
    [[nodiscard]] int getResult() const {
      return result;
    }
    [[nodiscard]] std::string getMessage() const {
      return message;
    }
    [[nodiscard]] long getTimeTaken() const {
      return time_taken;
    }
  private:
    std::string operation;
    int result;
    std::string message;
    long time_taken;
};
