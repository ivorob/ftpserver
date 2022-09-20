#pragma once

#include "Socket.h"

class DataConnection {
public:
    DataConnection(const struct sockaddr_in& address);

    struct sockaddr_in getAddress() const;
private:
    Socket dataSocket;
};
