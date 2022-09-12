#include <stdio.h>
#include <filesystem>

#include "OSApiImpl.h"

namespace fs = std::filesystem;

int OSApiImpl::socket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

int OSApiImpl::setsockopt(int s, int level, int optname, 
    void* optval, socklen_t optlen)
{
    return ::setsockopt(s, level, optname, optval, optlen);
}

int OSApiImpl::getsockname(int s, struct sockaddr* name, socklen_t* namelen)
{
    return ::getsockname(s, name, namelen);
}

ssize_t OSApiImpl::recv(int s, void* buf, size_t len, int flags)
{
    return ::recv(s, buf, len, flags);
}

ssize_t OSApiImpl::send(int s, const void* msg, size_t len, int flags)
{
    return ::send(s, msg, len, flags);
}

int OSApiImpl::bind(int s, const struct sockaddr* addr, socklen_t addrlen)
{
    return ::bind(s, addr, addrlen);
}

int OSApiImpl::listen(int s, int backlog)
{
    return ::listen(s, backlog);
}

int OSApiImpl::select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
    struct timeval* timeout)
{
    return ::select(nfds, readfds, writefds, exceptfds, timeout);
}

int OSApiImpl::accept(int s, struct sockaddr* addr, socklen_t* addrlen)
{
    return ::accept(s, addr, addrlen);
}

int OSApiImpl::remove(const char* path)
{
    return ::remove(path);
}

int OSApiImpl::close(int fd)
{
    return ::close(fd);
}

int OSApiImpl::rmdir(const char* path)
{
    if (path != nullptr) {
        std::error_code errorCode;
        fs::remove_all(path, errorCode);
        return errorCode.value() == 0;
    }

    return -1;
}

int OSApiImpl::chdir(const char* path)
{
    if (path != nullptr) {
        std::error_code errorCode;
        fs::current_path(path, errorCode);
        return errorCode.value() == 0;
    }

    return -1;
}

bool OSApiImpl::canOpenDirectory(const std::string& path) const
{
    std::error_code errorCode;
    if (fs::is_directory(path, errorCode)) {
        auto oldPath = fs::current_path();

        fs::current_path(path, errorCode);
        bool isChanged = errorCode.value() == 0;
        fs::current_path(oldPath, errorCode);
        return isChanged;
    }

    return false;
}

int OSApiImpl::mkdir(const char* path, mode_t mode)
{
    //TODO: use mode parameter
    if (path != nullptr) {
        std::error_code errorCode;
        fs::create_directories(path, errorCode);
        return errorCode.value() == 0;
    }

    return -1;
}
