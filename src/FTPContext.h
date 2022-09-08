#pragma once

#include <functional>

#include "fileoperator.h"

namespace FTP {

class Context {
public:
    std::function<void()> shutdownConnection;
    std::function<std::string()> getCommand;

    std::weak_ptr<fileoperator> fileoperator;
};

}
