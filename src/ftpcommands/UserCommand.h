#pragma once

#include "AbstractFTPCommand.h"

namespace FTP {

class UserCommand : public AbstractCommand {
public:
    std::string execute(std::vector<std::string> arguments) override;
private:
    const std::string command;
};

}
