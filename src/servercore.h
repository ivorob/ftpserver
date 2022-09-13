#pragma once

#include <string>
#include <list>
#include <memory>

#include "serverconnection.h"
#include "Socket.h"

class servercore {
public:
    servercore(uint16_t port,std::string dir, unsigned short commandOffset = 1);
    ~servercore();

private:
    int start();
    void initSockets(int port);
    void buildSelectList();
    void readSockets();
    int handleNewConnection();
private:
    void shutdownServer();
private:
    unsigned int maxConnectionsInQuery; // number of connections in query
    Socket listenSocket;
    int sflags; // Socket fd flags
    std::list<std::unique_ptr<serverconnection>> connections; // Manage the connected sockets / connections in a list with an iterator
    int highSock; // Highest #'d file descriptor, needed for select()
    fd_set socks; // set of socket file descriptors we want to wake up for, using select()
    std::string dir;
    unsigned int connId;
    bool shutdown;
    sockaddr_in addr;
    struct sockaddr_storage addrStorage;
    socklen_t addrLength;
    unsigned short commandOffset;
};
