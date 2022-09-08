#include <iostream>

#include "CurrentDirectoryCommand.h"

FTP::CurrentDirectoryCommand::CurrentDirectoryCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::CurrentDirectoryCommand::execute(std::vector<std::string> arguments) {
    std::cout << "Working dir requested" << std::endl;

    auto fileoperator = getContext().fileoperator.lock();
    if (fileoperator) {
        return "257 \"" + fileoperator->getCurrentWorkingDir(false) + "\"";
    }

    return ""; //TODO: error code
}
