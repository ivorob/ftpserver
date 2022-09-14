#pragma once

#include "os.h"

class Socket {
public:
    Socket() = default;
    explicit Socket(SOCKET sockfd);
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&&);
    Socket& operator=(Socket&&);
    virtual ~Socket();

    int allowReuse();
    int makeNonBlocking();
    void bind(struct sockaddr_in& addr);
    void listen(int backlog);

    struct sockaddr_in getAddress() const;

    Socket accept();

    int native() const;
private:
    void closeSocket();
private:
    SOCKET sockfd = INVALID_SOCKET;
    struct sockaddr_in address = {0};
};
