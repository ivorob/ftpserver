#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class SystemTypeCommand : public Command {
public:
    SystemTypeCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
