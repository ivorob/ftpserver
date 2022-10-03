#pragma once

#include <string>

#include "Socket.h"

class Transmitter {
public:
    Transmitter(Socket& dataSocket, std::string data);

    ssize_t transmit();
private:
    Socket& dataSocket;
    std::string data;
};
