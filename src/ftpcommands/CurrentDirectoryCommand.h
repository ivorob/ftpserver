#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class CurrentDirectoryCommand : public Command {
public:
    CurrentDirectoryCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
