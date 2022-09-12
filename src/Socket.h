#pragma once

#include <netinet/in.h>

class Socket {
public:
    Socket() = default;
    explicit Socket(int sockfd);
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&&);
    Socket& operator=(Socket&&);
    virtual ~Socket();

    int allowReuse();
    void bind(struct sockaddr_in& addr);
    void listen(int backlog);

    struct sockaddr_in getAddress() const;

    Socket accept();

    int native() const;
private:
    void closeSocket();
private:
    int sockfd = -1;
    struct sockaddr_in address = {0};
};
