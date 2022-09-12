#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class PassiveModeCommand : public Command {
public:
    PassiveModeCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
