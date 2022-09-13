#pragma once

#include "AbstractOSApi.h"

class OSApiImpl : public AbstractOSApi {
public:
    OSApiImpl();

    int socket(int domain, int type, int protocol) override;
    int setsockopt(int s, int level, int optname, 
        void* optval, socklen_t optlen) override;
    int getsockname(int s, struct sockaddr* name, socklen_t* namelen) override;
    ssize_t recv(int s, void* buf, size_t len, int flags) override;
    ssize_t send(int s, const void* msg, size_t len, int flags) override;
    int bind(int s, const struct sockaddr* addr, socklen_t addrlen) override;
    int listen(int s, int backlog) override;
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
        struct timeval* timeout) override;
    int accept(int s, struct sockaddr* addr, socklen_t* addrlen) override;
    int remove(const char* path) override;

    int close(SOCKET fd) override;
    int rmdir(const char* path) override;
    int chdir(const char* path) override;
    bool canOpenDirectory(const std::string& path) const override;

    int mkdir(const char* path, uint16_t mode) override;
};
