#include <stdio.h>
#include <filesystem>

#include "OSApiImpl.h"

namespace fs = std::filesystem;

OSApiImpl::OSApiImpl()
{
#if defined(_WIN32) || defined(_WIN64)
    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif
}

SOCKET OSApiImpl::socket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

int OSApiImpl::setsockopt(SOCKET s, int level, int optname, 
    void* optval, socklen_t optlen)
{
#if defined(_WIN32) || defined(_WIN64)
    return ::setsockopt(s, level, optname, reinterpret_cast<const char*>(optval), optlen);
#else
    return ::setsockopt(s, level, optname, optval, optlen);
#endif
}

int OSApiImpl::getsockname(SOCKET s, struct sockaddr* name, socklen_t* namelen)
{
    return ::getsockname(s, name, namelen);
}

ssize_t OSApiImpl::recv(SOCKET s, void* buf, size_t len, int flags)
{
#if defined(_WIN32) || defined(_WIN64)
    return ::recv(s, reinterpret_cast<char*>(buf), static_cast<int>(len), flags);
#else
    return ::recv(s, buf, len, flags);
#endif
}

ssize_t OSApiImpl::send(SOCKET s, const void* msg, size_t len, int flags)
{
#if defined(_WIN32) || defined(_WIN64)
    return ::send(s, reinterpret_cast<const char*>(msg), static_cast<int>(len), flags);
#else
    return ::send(s, msg, len, flags);
#endif
}

int OSApiImpl::bind(SOCKET s, const struct sockaddr* addr, socklen_t addrlen)
{
    return ::bind(s, addr, addrlen);
}

int OSApiImpl::listen(SOCKET s, int backlog)
{
    return ::listen(s, backlog);
}

int OSApiImpl::select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
    struct timeval* timeout)
{
    return ::select(nfds, readfds, writefds, exceptfds, timeout);
}

SOCKET OSApiImpl::accept(SOCKET s, struct sockaddr* addr, socklen_t* addrlen)
{
    return ::accept(s, addr, addrlen);
}

int OSApiImpl::remove(const char* path)
{
    return ::remove(path);
}

int OSApiImpl::close(SOCKET fd)
{
#if defined(_WIN32) || defined(_WIN64)
    return ::closesocket(fd);
#else
    return ::close(fd);
#endif
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

int OSApiImpl::mkdir(const char* path, uint16_t mode)
{
    //TODO: use mode parameter
    if (path != nullptr) {
        std::error_code errorCode;
        fs::create_directories(path, errorCode);
        return errorCode.value() == 0;
    }

    return -1;
}
