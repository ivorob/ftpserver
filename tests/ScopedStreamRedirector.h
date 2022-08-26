#pragma once

#include <ostream>

class ScopedStreamRedirector {
public:
    ScopedStreamRedirector(const ScopedStreamRedirector&) = delete;
    ScopedStreamRedirector& operator=(const ScopedStreamRedirector&) = delete;

    ScopedStreamRedirector(std::ostream& in, std::ostream& out);
    ~ScopedStreamRedirector();
private:
    std::ostream& in;
    std::streambuf* buffer = nullptr;
};
