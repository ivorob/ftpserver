#include <iostream>

#include "UserCommand.h"
#include "Logger.h"

FTP::UserCommand::UserCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::UserCommand::execute(std::vector<std::string> arguments) {
    if (!arguments.empty()) {
        LOG(logger, Logger::LogLevel::Info) << "User '" + arguments.at(0) + "' is logged successfully!" << std::endl;
    }
    
    return "230 User logged in, proceed.";
}
