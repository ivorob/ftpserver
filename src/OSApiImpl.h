#pragma once

#include "AbstractOSApi.h"

class OSApiImpl : public AbstractOSApi {
public:
    OSApiImpl();

    SOCKET socket(int domain, int type, int protocol) override;
    int setsockopt(SOCKET s, int level, int optname, 
        void* optval, socklen_t optlen) override;
    int getsockname(SOCKET s, struct sockaddr* name, socklen_t* namelen) override;
    ssize_t recv(SOCKET s, void* buf, size_t len, int flags) override;
    ssize_t send(SOCKET s, const void* msg, size_t len, int flags) override;
    int bind(SOCKET s, const struct sockaddr* addr, socklen_t addrlen) override;
    int listen(SOCKET s, int backlog) override;
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
        struct timeval* timeout) override;
    SOCKET accept(SOCKET s, struct sockaddr* addr, socklen_t* addrlen) override;
    bool makeNonBlocking(SOCKET s) override;
    int remove(const char* path) override;

    int close(SOCKET fd) override;
    int rmdir(const char* path) override;
    int chdir(const char* path) override;
    bool canOpenDirectory(const std::string& path) const override;

    int mkdir(const char* path, uint16_t mode) override;
};
