#include <unistd.h>
#include <sys/stat.h>

#include "OSApiImpl.h"

int OSApiImpl::socket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
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

int OSApiImpl::close(int fd)
{
    return ::close(fd);
}

int OSApiImpl::rmdir(const char* path)
{
    return ::rmdir(path);
}

int OSApiImpl::chdir(const char* path)
{
    return ::chdir(path);
}

DIR* OSApiImpl::opendir(const char* filename)
{
    return ::opendir(filename);
}

struct dirent* OSApiImpl::readdir(DIR* dirp)
{
    return ::readdir(dirp);
}

int OSApiImpl::closedir(DIR* dirp)
{
    return ::closedir(dirp);
}

int OSApiImpl::mkdir(const char* path, mode_t mode)
{
    return ::mkdir(path, mode);
}
