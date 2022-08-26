#include "ScopedStreamRedirector.h"

ScopedStreamRedirector::ScopedStreamRedirector(std::ostream& in, std::ostream& out)
    : in(in)
    , buffer(in.rdbuf(out.rdbuf()))
{
}

ScopedStreamRedirector::~ScopedStreamRedirector()
{
    in.rdbuf(buffer);
}
