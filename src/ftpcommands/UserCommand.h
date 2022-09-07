#pragma once

#include "AbstractFTPCommand.h"
#include "FTPCommand.h"

namespace FTP {

class UserCommand : public Command {
public:
    UserCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
