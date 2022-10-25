#pragma once

#include <fstream>
#include "LogHandler.h"

class NullLogHandler : public LogHandler {
public:
    NullLogHandler();

    std::ostream& getStream() override;
private:
    std::ofstream output;
};
