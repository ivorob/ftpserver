#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <dirent.h>

namespace OSApi {

int socketImpl(int domain, int type, int protocol);
ssize_t recvImpl(int s, void* buf, size_t len, int flags);
ssize_t sendImpl(int s, const void* msg, size_t len, int flags);
int bindImpl(int s, const struct sockaddr* addr, socklen_t addrlen);
int listenImpl(int s, int backlog);
int selectImpl(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
    struct timeval* timeout);
int acceptImpl(int s, struct sockaddr* addr, socklen_t* addrlen);

int closeImpl(int fd);
int rmdirImpl(const char* path);
int chdirImpl(const char* path);

DIR *opendirImpl(const char* filename);
struct dirent* readdirImpl(DIR* dirp);
int closedirImpl(DIR* dirp);

int mkdirImpl(const char* path, mode_t mode);

// sys/socket.h
const auto socket = socketImpl;
const auto recv = recvImpl;
const auto send = sendImpl;
const auto bind = bindImpl;
const auto listen = listenImpl;
const auto select = selectImpl;
const auto accept = acceptImpl;

// unistd.h
const auto close = closeImpl;
const auto rmdir = rmdirImpl;
const auto chdir = chdirImpl;

// dirent.h
const auto opendir = opendirImpl;
const auto readdir = readdirImpl;
const auto closedir = closedirImpl;

// sys/stat.h
const auto mkdir = mkdirImpl;

}
