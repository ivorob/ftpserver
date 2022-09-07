#include <iostream>

#include "UserCommand.h"

FTP::UserCommand::UserCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::UserCommand::execute(std::vector<std::string> arguments) {
    if (!arguments.empty()) {
        std::cout << "User '" + arguments.at(0) + "' is logged successfully!" << std::endl;
    }
    
    return "230 User logged in, proceed.";
}
