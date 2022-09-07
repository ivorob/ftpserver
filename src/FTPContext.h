#pragma once

#include <functional>

namespace FTP {

class Context {
public:
    std::function<void()> shutdownConnection;
    std::function<std::string()> getCommand;
};

}
