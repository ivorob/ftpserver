#pragma once

#include <gmock/gmock.h>

#include "AbstractOSApi.h"

class MockOSApiImpl : public AbstractOSApi {
public:
    MOCK_METHOD3(socket, int(int,int,int));
    MOCK_METHOD5(setsockopt, int(int, int, int, void*, socklen_t));
    MOCK_METHOD4(recv, ssize_t(int, void*, size_t, int));
    MOCK_METHOD4(send, ssize_t(int, const void*, size_t, int));
    MOCK_METHOD3(bind, int(int, const struct sockaddr*, socklen_t));
    MOCK_METHOD2(listen, int(int, int));
    MOCK_METHOD5(select, int(int, fd_set*, fd_set*, fd_set*, struct timeval*));
    MOCK_METHOD3(accept, int(int, struct sockaddr*, socklen_t*));
    MOCK_METHOD1(remove, int(const char*));

    MOCK_METHOD1(close, int(int));
    MOCK_METHOD1(rmdir, int(const char*));
    MOCK_METHOD1(chdir, int(const char*));

    MOCK_METHOD1(opendir, DIR*(const char*));
    MOCK_METHOD1(readdir, struct dirent*(DIR*));
    MOCK_METHOD1(closedir, int(DIR*));

    MOCK_METHOD2(mkdir, int(const char*, mode_t));
};

