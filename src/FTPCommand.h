#pragma once

#include "AbstractFTPCommand.h"
#include "FTPContext.h"

namespace FTP {

class Command : public AbstractCommand {
public:
    Command(Context context);

    Context& getContext();
private:
    Context context;
};

}
