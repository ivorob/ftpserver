#include <stdexcept>

#include "Transmitter.h"
#include "OSApi.h"

Transmitter::Transmitter(Socket& dataSocket, std::string data)
    : dataSocket(dataSocket)
    , data(std::move(data)) {
}

ssize_t Transmitter::transmit() {
    const size_t dataSize = data.size();
    size_t position = 0;
    while (position < dataSize) {
        ssize_t bytesSent = api()->send(dataSocket.native(), &data[position], 
                dataSize - position, 0);
        if (bytesSent == -1) {
            throw std::runtime_error("Cannot send data");
        }

        position += static_cast<size_t>(bytesSent);
    }

    return position;
}
