#pragma once

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm> // for transform command
#include <memory>

#include "fileoperator.h"
#include "Socket.h"
#include "FTPCommandFactory.h"
#include "FTPContext.h"
#include "DataConnection.h"

// Separator for commands
#define SEPARATOR " "

class serverconnection {
public:
    serverconnection(Socket currentSocket, unsigned int connId, std::string defaultDir, std::string hostId, unsigned short commandOffset = 1);
    std::string commandParser(std::string command);
    std::vector<std::string> extractParameters(std::string command);
    ~serverconnection();
    void respondToQuery();
    SOCKET getFD() const;
    bool getCloseRequestStatus() const;
    unsigned int getConnectionId() const;
private:
    FTP::Context makeContext();

    void shutdown();
    struct sockaddr_in obtainDataConnection();

    void sendToClient(const char* response, unsigned long length);
    void sendToClient(std::string response);
    bool commandEquals(std::string a, std::string b);
    std::string filterOutBlanks(std::string inString);
    static void getAllParametersAfter(std::vector<std::string> parameterVector, unsigned int currentParameter, std::string& theRest);
private:
    std::shared_ptr<fileoperator> fo; // For browsing, writing and reading
    Socket currentSocket;
    FTP::CommandFactory ftpCommandFactory;
    bool closureRequested;
    std::vector<std::string> directories;
    std::vector<std::string> files;
    unsigned int connectionId;
    std::string dir;
    std::string hostAddress;
    unsigned short commandOffset;
    std::unique_ptr<DataConnection> dataConnection;
};
