#include <unistd.h>

#include "OSApiFactory.h"

int OSApi::socketImpl(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

ssize_t OSApi::recvImpl(int s, void* buf, size_t len, int flags)
{
    return ::recv(s, buf, len, flags);
}

ssize_t OSApi::sendImpl(int s, const void* msg, size_t len, int flags)
{
    return ::send(s, msg, len, flags);
}

int OSApi::bindImpl(int s, const struct sockaddr* addr, socklen_t addrlen)
{
    return ::bind(s, addr, addrlen);
}

int OSApi::listenImpl(int s, int backlog)
{
    return ::listen(s, backlog);
}

int OSApi::selectImpl(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
    struct timeval* timeout)
{
    return ::select(nfds, readfds, writefds, exceptfds, timeout);
}

int OSApi::acceptImpl(int s, struct sockaddr* addr, socklen_t* addrlen)
{
    return ::accept(s, addr, addrlen);
}

int OSApi::closeImpl(int fd)
{
    return ::close(fd);
}

int OSApi::rmdirImpl(const char* path)
{
    return ::rmdir(path);
}

int OSApi::chdirImpl(const char* path)
{
    return ::chdir(path);
}

DIR* OSApi::opendirImpl(const char* filename)
{
    return ::opendir(filename);
}

struct dirent* OSApi::readdirImpl(DIR* dirp)
{
    return ::readdir(dirp);
}

int OSApi::closedirImpl(DIR* dirp)
{
    return ::closedir(dirp);
}

int OSApi::mkdirImpl(const char* path, mode_t mode)
{
    return ::mkdir(path, mode);
}
