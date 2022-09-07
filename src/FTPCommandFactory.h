#pragma once

#include <memory>
#include <utility>

#include "AbstractFTPCommand.h"
#include "FTPContext.h"

namespace FTP {

using CommandHolder = std::unique_ptr<AbstractCommand>;

class CommandFactory {
public:
    CommandFactory(Context context);

    CommandHolder create(const std::string& command);
private:
    Context context;
};

}

