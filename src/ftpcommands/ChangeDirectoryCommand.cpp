#include <iostream>

#include "ChangeDirectoryCommand.h"

FTP::ChangeDirectoryCommand::ChangeDirectoryCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::ChangeDirectoryCommand::execute(std::vector<std::string> arguments) {
    //TODO: check if argument is present
    auto newDirectory = arguments.at(0);
    std::cout << "Change of working dir to '" << newDirectory << "' requested" << std::endl;

    auto fileOperator = getContext().fileOperator.lock();
    if (fileOperator) {
        // Test if dir exists
        if (!fileOperator->changeDir(newDirectory)) {
            std::cout << "Directory change to '" << newDirectory << "' successful!" << std::endl;

            return "250 directory changed to " + fileOperator->getCurrentWorkingDir(false);
        }
    }

    return ""; //TODO: error code
}
