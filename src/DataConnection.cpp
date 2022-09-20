#include "DataConnection.h"
#include "OSApi.h"

DataConnection::DataConnection(const struct sockaddr_in& address)
    : dataSocket(api()->socket(AF_INET, SOCK_STREAM, 0)) {

    struct sockaddr_in newAddress(address);
    newAddress.sin_port = 0;
    dataSocket.bind(newAddress);
}

struct sockaddr_in DataConnection::getAddress() const {
    return dataSocket.getAddress();
}
