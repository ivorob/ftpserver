#include "StringLogHandler.h"

StringLogHandler::StringLogHandler(std::ostream& output)
    : output(output) {
}

std::ostream&
StringLogHandler::getStream() {
    return this->output;
}
