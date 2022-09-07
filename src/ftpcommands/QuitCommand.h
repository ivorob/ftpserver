#pragma once

#include "FTPCommand.h"

namespace FTP {

class QuitCommand : public Command {
public:
    QuitCommand(Context context);

    std::string execute(std::vector<std::string> arguments) override;
};

}
