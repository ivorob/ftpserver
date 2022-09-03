#pragma once

#include "AbstractFTPCommand.h"

namespace FTP {

class DefaultCommand : public AbstractCommand {
public:
    DefaultCommand(std::string command);

    std::string execute(std::vector<std::string> arguments) override;
private:
    const std::string command;
};

}
