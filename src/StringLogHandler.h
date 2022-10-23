#pragma once

#include <sstream>

#include "LogHandler.h"

class StringLogHandler : public LogHandler {
public:
    StringLogHandler(std::ostream& output);

    std::ostream& getStream() override;
private:
    std::ostream& output;
};
