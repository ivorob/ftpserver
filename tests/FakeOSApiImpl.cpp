#include "FakeOSApiImpl.h"

int DummyOSApi::socket(int, int, int)
{
    return 0;
}

ssize_t DummyOSApi::recv(int, void*, size_t, int)
{
    return -1;
}

ssize_t DummyOSApi::send(int, const void*, size_t, int)
{
    return -1;
}

int DummyOSApi::bind(int, const struct sockaddr*, socklen_t)
{
    return -1;
}

int DummyOSApi::listen(int, int)
{
    return -1;
}

int DummyOSApi::select(int, fd_set*, fd_set*, fd_set*,
    struct timeval*)
{
    return -1;
}

int DummyOSApi::accept(int, struct sockaddr*, socklen_t*)
{
    return -1;
}

int DummyOSApi::close(int)
{
    return -1;
}

int DummyOSApi::rmdir(const char*)
{
    return -1;
}

int DummyOSApi::chdir(const char*)
{
    return -1;
}

DIR* DummyOSApi::opendir(const char*) 
{
    return nullptr;
}

struct dirent* DummyOSApi::readdir(DIR*)
{
    return nullptr;
}

int DummyOSApi::closedir(DIR*)
{
    return -1;
}

int DummyOSApi::mkdir(const char*, mode_t)
{
    return -1;
}
