#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class DefaultCommand : public Command {
public:
    DefaultCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
