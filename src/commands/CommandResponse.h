#pragma once

#include <string>
#include <utility>

class CommandResponse {
  public:
    CommandResponse(std::string operation, int result, std::string message, long timeTaken) : operation(std::move(operation)),result(result),message(std::move(message)),time_taken(timeTaken) {}
    inline std::string getOperation() { return operation; }
    [[nodiscard]] inline int getResult() const { return result; }
    inline std::string getMessage() { return message; }
    [[nodiscard]] inline long getTimeTaken() const { return time_taken; }
  private:
    std::string operation;
    int result;
    std::string message;
    long time_taken;
};
