#pragma once

#include <functional>
#include <memory>

#include "fileoperator.h"

namespace FTP {

class Context {
public:
    std::function<void()> shutdownConnection;
    std::function<std::string()> getCommand;
    std::function<struct sockaddr_in()> obtainDataConnection;

    std::weak_ptr<fileoperator> fileOperator;
};

}
