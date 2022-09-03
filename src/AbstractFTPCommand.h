#pragma once

#include <string>
#include <vector>

namespace FTP {

class AbstractCommand {
public:
    virtual ~AbstractCommand() = default;

    virtual std::string execute(std::vector<std::string> arguments) = 0;
};

}
