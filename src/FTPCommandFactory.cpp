#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iterator>

#include "FTPCommandFactory.h"
#include "FTPContext.h"
#include "ftpcommands/Commands.h"

namespace {

template <typename T>
FTP::CommandHolder createCommand(FTP::Context&& context)
{
    return std::unique_ptr<T>(new T(std::forward<FTP::Context>(context)));
}

std::unordered_map<std::string, std::function<FTP::CommandHolder(FTP::Context&&)>> ftpCommandMap
{
    { "USER",   createCommand<FTP::UserCommand> },
    { "BYE",    createCommand<FTP::QuitCommand> },
    { "QUIT",   createCommand<FTP::QuitCommand> },
    { "PWD",    createCommand<FTP::CurrentDirectoryCommand> },
    { "CWD",    createCommand<FTP::ChangeDirectoryCommand> },
    { "SYST",   createCommand<FTP::SystemTypeCommand> },
    { "PASV",   createCommand<FTP::PassiveModeCommand> },
};

}

FTP::CommandFactory::CommandFactory(Context context)
    : context(context) {
}

FTP::CommandHolder FTP::CommandFactory::create(const std::string& command)
{
    std::string upperCaseCommand;
    std::transform(command.begin(), command.end(), 
        std::back_inserter(upperCaseCommand), toupper);

    auto contextCopy = context;
    contextCopy.getCommand = [command]() {
        return command;
    };

    auto it = ftpCommandMap.find(upperCaseCommand);
    if (it != ftpCommandMap.cend()) {
        auto createCommand = it->second;
        return createCommand(std::move(contextCopy));
    }

    return createCommand<FTP::DefaultCommand>(std::move(contextCopy));
}
