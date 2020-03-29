//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDINTERFACE_H
#define SMV_COMMANDINTERFACE_H

#include <string>
#include <utility>

class commandInterface {
    std::string operation;
public:
    explicit commandInterface(std::string operation_name){
        operation = std::move(operation_name);
    }
    virtual bool parse(std::string line) = 0;
    virtual std::string perform() = 0;
};
#endif //SMV_COMMANDINTERFACE_H
