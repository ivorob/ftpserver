#include <stdexcept>

#include "Socket.h"
#include "OSApi.h"

Socket::Socket(SOCKET sockfd)
    : sockfd(sockfd)
{
    if (this->sockfd == INVALID_SOCKET) {
        throw std::runtime_error("socket() failed");
    }

    socklen_t namelen = sizeof(address);
    if (api()->getsockname(this->sockfd, 
            reinterpret_cast<struct sockaddr*>(&address), 
            &namelen) == -1) {
        closeSocket();
        throw std::runtime_error("Cannot get socket address");
    }
}

Socket::~Socket()
{
    closeSocket();
}

void Socket::closeSocket()
{
    if (this->sockfd != INVALID_SOCKET) {
        api()->close(this->sockfd);
        this->sockfd = INVALID_SOCKET;
    }
}

Socket::Socket(Socket&& other)
    : sockfd(other.sockfd)
    , address(other.address)
{
    other.sockfd = INVALID_SOCKET;
    other.address = {0};
}

Socket& Socket::operator=(Socket&& other)
{
    if (this->sockfd != other.sockfd) {
        closeSocket();

        std::swap(this->sockfd, other.sockfd);

        this->address = {0};
        std::swap(this->address, other.address);
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

int Socket::makeNonBlocking()
{
    if (this->sockfd < 0) {
        return -1;
    }

    if (!api()->makeNonBlocking(this->sockfd)) {
        throw std::runtime_error("Error settings socket to non-blocking");
    }

    return 0;
}

void Socket::bind(const struct sockaddr_in& addr)
{
    if (api()->bind(this->sockfd, 
            reinterpret_cast<const struct sockaddr*>(&addr),
            sizeof(struct sockaddr_in)) == -1) {
        throw std::runtime_error("bind() failed (do you have the apropriate rights? is the port unused?)");
    }

    socklen_t namelen = sizeof(address);
    if (api()->getsockname(this->sockfd, 
            reinterpret_cast<struct sockaddr*>(&address), 
            &namelen) == -1) {
        throw std::runtime_error("Cannot get socket address");
    }
}

void Socket::listen(int backlog)
{
    if (api()->listen(this->sockfd, backlog) == -1) {
        throw std::runtime_error("listen () failed");
    }
}

struct sockaddr_in Socket::getAddress() const
{
    return this->address;
}

Socket Socket::accept()
{
    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    return Socket(api()->accept(this->sockfd, 
                reinterpret_cast<struct sockaddr*>(&addr), &size));
}

SOCKET Socket::native() const
{
    return this->sockfd;
}
