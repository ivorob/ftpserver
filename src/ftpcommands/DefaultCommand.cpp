#include <iostream>

#include "DefaultCommand.h"
#include "Logger.h"

FTP::DefaultCommand::DefaultCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::DefaultCommand::execute(std::vector<std::string>) {
    LOG(logger, Logger::LogLevel::Info) << "Unknown command encountered '" << getContext().getCommand() << "'!" << std::endl;
    return "502 Command not implemented.";
}
