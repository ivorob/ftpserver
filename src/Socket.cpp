#include <stdexcept>

#include "Socket.h"
#include "OSApi.h"

Socket::Socket(int sockfd)
    : sockfd(sockfd)
{
    if (this->sockfd == -1) {
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
    if (this->sockfd >= 0) {
        api()->close(this->sockfd);
        this->sockfd = -1;
    }
}

Socket::Socket(Socket&& other)
    : sockfd(other.sockfd)
    , address(other.address)
{
    other.sockfd = -1;
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

int Socket::native() const
{
    return this->sockfd;
}
