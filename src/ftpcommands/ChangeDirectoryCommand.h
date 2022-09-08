#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class ChangeDirectoryCommand : public Command {
public:
    ChangeDirectoryCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
