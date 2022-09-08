#include <iostream>

#include "CurrentDirectoryCommand.h"

FTP::CurrentDirectoryCommand::CurrentDirectoryCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::CurrentDirectoryCommand::execute(std::vector<std::string> arguments) {
    std::cout << "Working dir requested" << std::endl;

    auto fileOperator = getContext().fileOperator.lock();
    if (fileOperator) {
        return "257 \"" + fileOperator->getCurrentWorkingDir(false) + "\"";
    }

    return ""; //TODO: error code
}
