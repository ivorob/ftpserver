#pragma once

#include <ostream>

class LogHandler {
public:
    virtual ~LogHandler() = default;

    virtual std::ostream& getStream() = 0;
};
