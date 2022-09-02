#include <stdexcept>

#include "Socket.h"
#include "OSApi.h"

Socket::Socket(int sockfd)
    : sockfd(sockfd)
{
    if (this->sockfd == -1) {
        throw std::runtime_error("socket() failed");
    }
}

Socket::~Socket()
{
    closeSocket();
}

void Socket::closeSocket()
{
    if (this->sockfd >= 0) {
        api()->close(this->sockfd);
        this->sockfd = -1;
    }
}

Socket::Socket(Socket&& other)
    : sockfd(other.sockfd)
{
    other.sockfd = -1;
}

Socket& Socket::operator=(Socket&& other)
{
    if (this->sockfd != other.sockfd) {
        closeSocket();

        std::swap(this->sockfd, other.sockfd);
    }

    return *this;
}

int Socket::allowReuse()
{
    if (this->sockfd < 0) {
        return -1;
    }

    int reuseAllowed = 1;
    return api()->setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR,
        &reuseAllowed, sizeof(reuseAllowed));
}

void Socket::bind(struct sockaddr_in& addr)
{
    if (api()->bind(this->sockfd, 
            reinterpret_cast<struct sockaddr*>(&addr),
            sizeof(struct sockaddr_in)) == -1) {
        throw std::runtime_error("bind() failed (do you have the apropriate rights? is the port unused?)");
    }
}

void Socket::listen(int backlog)
{
    if (api()->listen(this->sockfd, backlog) == -1) {
        throw std::runtime_error("listen () failed");
    }
}

Socket Socket::accept()
{
    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    return Socket(api()->accept(this->sockfd, 
                reinterpret_cast<struct sockaddr*>(&addr), &size));
}

int Socket::native() const
{
    return this->sockfd;
}
