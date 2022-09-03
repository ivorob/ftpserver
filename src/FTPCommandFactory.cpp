#include <unordered_map>

#include "FTPCommandFactory.h"
#include "ftpcommands/Commands.h"

namespace {

std::unordered_map<std::string, std::function<FTP::CommandHolder()>> ftpCommandMap
{
    { "USER",   FTP::createCommand<FTP::UserCommand> },
};

}

FTP::CommandHolder
FTP::CommandFactory::create(const std::string& command)
{
    std::string upperCaseCommand;
    std::transform(command.begin(), command.end(), 
        std::back_inserter(upperCaseCommand), toupper);

    auto it = ftpCommandMap.find(upperCaseCommand);
    if (it != ftpCommandMap.cend()) {
        auto createCommand = it->second;
        return createCommand();
    }

    return createCommand<FTP::DefaultCommand>(command);
}
