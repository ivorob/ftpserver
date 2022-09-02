#pragma once

#include <netinet/in.h>

class Socket {
public:
    Socket() = default;
    Socket(int sockfd);
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&&);
    Socket& operator=(Socket&&);
    ~Socket();

    int allowReuse();
    void bind(struct sockaddr_in& addr);
    void listen(int backlog);

    Socket accept();

    int native() const;
private:
    void closeSocket();
private:
    int sockfd = -1;
};
