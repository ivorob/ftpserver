#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <iostream>
#include <algorithm>

#include "PassiveModeCommand.h"

FTP::PassiveModeCommand::PassiveModeCommand(Context context)
    : Command(std::move(context)) {
}

std::string FTP::PassiveModeCommand::execute(std::vector<std::string>) {
    struct sockaddr_in address = getContext().obtainDataConnection();
    uint16_t port = htons(address.sin_port);

    char buffer[128];
    std::cout << "Initiate data connection on " 
              << inet_ntop(AF_INET, &address.sin_addr.s_addr, buffer, sizeof(buffer))
              << ":" << port << std::endl;

    std::string ipv4Address(buffer);
    std::replace(ipv4Address.begin(), ipv4Address.end(), '.', ',');
    return "227 Entering Passive Mode (" + ipv4Address + "," + 
        std::to_string(port / 256) + "," + 
        std::to_string(port % 256) + ")";
}
