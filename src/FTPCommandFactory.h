#pragma once

#include <memory>
#include <utility>

#include "AbstractFTPCommand.h"

namespace FTP {

using CommandHolder = std::unique_ptr<AbstractCommand>;

class CommandFactory {
public:
    CommandHolder create(const std::string& command);
};

template <typename T, typename... Args>
FTP::CommandHolder createCommand(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

