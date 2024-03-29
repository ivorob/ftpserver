#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

using socklen_t = int;
using ssize_t = int;
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>

#define SOCKET      int
#define INVALID_SOCKET (-1)
#endif
