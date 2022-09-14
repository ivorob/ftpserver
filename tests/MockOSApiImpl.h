#pragma once

#include <gmock/gmock.h>

#include "AbstractOSApi.h"

class MockOSApiImpl : public AbstractOSApi {
public:
    MOCK_METHOD3(socket, SOCKET(int,int,int));
    MOCK_METHOD5(setsockopt, int(SOCKET, int, int, void*, socklen_t));
    MOCK_METHOD3(getsockname, int(SOCKET, struct sockaddr*, socklen_t*));
    MOCK_METHOD4(recv, ssize_t(SOCKET, void*, size_t, int));
    MOCK_METHOD4(send, ssize_t(SOCKET, const void*, size_t, int));
    MOCK_METHOD3(bind, int(SOCKET, const struct sockaddr*, socklen_t));
    MOCK_METHOD2(listen, int(SOCKET, int));
    MOCK_METHOD5(select, int(int, fd_set*, fd_set*, fd_set*, struct timeval*));
    MOCK_METHOD3(accept, SOCKET(SOCKET, struct sockaddr*, socklen_t*));
    MOCK_METHOD1(remove, int(const char*));

    MOCK_METHOD1(close, int(SOCKET));
    MOCK_METHOD1(rmdir, int(const char*));
    MOCK_METHOD1(chdir, int(const char*));
    MOCK_CONST_METHOD1(canOpenDirectory, bool(const std::string&));

    MOCK_METHOD2(mkdir, int(const char*, uint16_t));
};

