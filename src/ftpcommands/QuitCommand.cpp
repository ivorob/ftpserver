#include <iostream>

#include "QuitCommand.h"
#include "Logger.h"

FTP::QuitCommand::QuitCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::QuitCommand::execute(std::vector<std::string> arguments) {
    LOG(logger, Logger::LogLevel::Info) << "Shutdown of connection requested" << std::endl;
    getContext().shutdownConnection();
    return "221 Goodbye.";
}
