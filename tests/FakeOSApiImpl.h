#pragma once

#include "AbstractOSApi.h"

class DummyOSApi : public AbstractOSApi {
public:
    int socket(int domain, int type, int protocol) override;
    ssize_t recv(int s, void* buf, size_t len, int flags) override;
    ssize_t send(int s, const void* msg, size_t len, int flags) override;
    int bind(int s, const struct sockaddr* addr, socklen_t addrlen) override;
    int listen(int s, int backlog) override;
    int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
        struct timeval* timeout) override;
    int accept(int s, struct sockaddr* addr, socklen_t* addrlen) override;

    int close(int fd) override;
    int rmdir(const char* path) override;
    int chdir(const char* path) override;

    DIR *opendir(const char* filename) override;
    struct dirent* readdir(DIR* dirp) override;
    int closedir(DIR* dirp) override;

    int mkdir(const char* path, mode_t mode) override;
};
