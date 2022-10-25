#include "NullLogHandler.h"

NullLogHandler::NullLogHandler() {
#if defined(_WIN32) || defined(_WIN64)
    output.open("NUL:");
#else
    output.open("/dev/null");
#endif
}

std::ostream& NullLogHandler::getStream() {
    return this->output;
}
