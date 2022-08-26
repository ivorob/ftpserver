#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>

class AbstractOSApi {
public:
    virtual ~AbstractOSApi() = default;

    virtual int socket(int domain, int type, int protocol) = 0;
    virtual int setsockopt(int s, int level, int optname, 
        void* optval, socklen_t optlen) = 0;
    virtual ssize_t recv(int s, void* buf, size_t len, int flags) = 0;
    virtual ssize_t send(int s, const void* msg, size_t len, int flags) = 0;
    virtual int bind(int s, const struct sockaddr* addr, socklen_t addrlen) = 0;
    virtual int listen(int s, int backlog) = 0;
    virtual int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
        struct timeval* timeout) = 0;
    virtual int accept(int s, struct sockaddr* addr, socklen_t* addrlen) = 0;

    virtual int close(int fd) = 0;
    virtual int rmdir(const char* path) = 0;
    virtual int chdir(const char* path) = 0;

    virtual DIR *opendir(const char* filename) = 0;
    virtual struct dirent* readdir(DIR* dirp) = 0;
    virtual int closedir(DIR* dirp) = 0;

    virtual int mkdir(const char* path, mode_t mode) = 0;
};
