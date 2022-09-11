#include "OSUtils.h"

std::string OSUtils::getOSName()
{
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__APPLE_CC__) || defined(__MACH__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(__FreeBSD__)
    return "FreeBSD";
#elif defined(__unix) || defined(__unix__)
    return "Unix";
#endif

    return "Other";
}
