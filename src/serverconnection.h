#pragma once

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sys/fcntl.h>
#include <algorithm> // for transform command
#include <memory>

#include "fileoperator.h"
#include "Socket.h"
#include "FTPCommandFactory.h"
#include "FTPContext.h"

// Separator for commands
#define SEPARATOR " "

class serverconnection {
public:
    serverconnection(Socket currentSocket, unsigned int connId, std::string defaultDir, std::string hostId, unsigned short commandOffset = 1);
    std::string commandParser(std::string command);
    std::vector<std::string> extractParameters(std::string command);
    ~serverconnection();
    void respondToQuery();
    int getFD() const;
    bool getCloseRequestStatus() const;
    unsigned int getConnectionId() const;
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
    bool uploadCommand;
    bool downloadCommand;
    std::string parameter;
private:
    FTP::Context makeContext();

    void shutdown();

    void sendToClient(const char* response, unsigned long length);
    void sendToClient(std::string response);
    bool commandEquals(std::string a, std::string b);
    std::string filterOutBlanks(std::string inString);
    static void getAllParametersAfter(std::vector<std::string> parameterVector, unsigned int currentParameter, std::string& theRest);
    unsigned short commandOffset;
    unsigned long receivedPart;
};
