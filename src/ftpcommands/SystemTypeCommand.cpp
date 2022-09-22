#include <iostream>

#include "SystemTypeCommand.h"
#include "OSUtils.h"
#include "Logger.h"

FTP::SystemTypeCommand::SystemTypeCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::SystemTypeCommand::execute(std::vector<std::string>) {
    LOG(logger, Logger::LogLevel::Info) << "Get of system type requested" << std::endl;

    return "215 " + OSUtils::getOSName() + " system type";
}
