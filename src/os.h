#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

using close = closesocket;
using socklen_t = int;
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#endif