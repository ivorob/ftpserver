#pragma once

#include <string>
#include "os.h"

class AbstractOSApi {
public:
    virtual ~AbstractOSApi() = default;

    virtual SOCKET socket(int domain, int type, int protocol) = 0;
    virtual int setsockopt(SOCKET s, int level, int optname, 
        void* optval, socklen_t optlen) = 0;
    virtual int getsockname(SOCKET s, struct sockaddr* name, socklen_t* namelen) = 0;
    virtual ssize_t recv(SOCKET s, void* buf, size_t len, int flags) = 0;
    virtual ssize_t send(SOCKET s, const void* msg, size_t len, int flags) = 0;
    virtual int bind(SOCKET s, const struct sockaddr* addr, socklen_t addrlen) = 0;
    virtual int listen(SOCKET s, int backlog) = 0;
    virtual int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
        struct timeval* timeout) = 0;
    virtual SOCKET accept(SOCKET s, struct sockaddr* addr, socklen_t* addrlen) = 0;
    virtual bool makeNonBlocking(SOCKET s) = 0;
    virtual int remove(const char *path) = 0;

    virtual int close(SOCKET fd) = 0;
    virtual int rmdir(const char* path) = 0;
    virtual int chdir(const char* path) = 0;
    virtual bool canOpenDirectory(const std::string& path) const = 0;

    virtual int mkdir(const char* path, uint16_t mode) = 0;
};
