#include <iostream>

#include "DefaultCommand.h"

FTP::DefaultCommand::DefaultCommand(std::string command)
    : command(std::move(command)) {
}

std::string FTP::DefaultCommand::execute(std::vector<std::string>) {
    std::cout << "Unknown command encountered '" << command << "'!" << std::endl;
    return "502 Command not implemented.";
}
