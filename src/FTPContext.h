#pragma once

namespace FTP {

class Context {
public:
    std::function<void()> shutdownConnection;
    std::function<std::string()> getCommand;
};

}
