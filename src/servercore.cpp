#include "servercore.h"
#include "serverconnection.h"
#include "OSApi.h"

servercore::servercore(uint16_t port, std::string dir, unsigned short commandOffset) 
    : dir(dir)
    , commandOffset(commandOffset)
    , shutdown(false)
    , connId(0) {

    if (api()->chdir(dir.c_str())) {
        std::cerr << "Directory could not be changed to '" << dir << "'!" << std::endl;
    }

    this->initSockets(port);
    this->start();
}

// Free up used memory by cleaning up all the object variables;
servercore::~servercore() {
    std::cout << "Server shutdown" << std::endl;
    shutdownServer();
}

void servercore::shutdownServer() {
    this->shutdown = true;
}

// Builds the list of sockets to keep track on and removes the closed ones
/// @TODO: Crash if data is incoming over a closed socket connection???
void servercore::buildSelectList() {
    // First put together fd_set for select(), which consists of the sock variable in case a new connection is coming in, plus all the already accepted sockets
    // FD_ZERO() clears out the fd_set called socks, so that it doesn't contain any file descriptors
    FD_ZERO(&(this->socks));

    // FD_SET() adds the file descriptor "sock" to the fd_set so that select() will return if a connection comes in on that socket (in which case accept() is called, etc.)
    FD_SET(this->listenSocket.native(), &(this->socks));

    // Iterates over all the possible connections and adds those sockets to the fd_set and erases closed ones
    auto iter = this->connections.begin();
    while (iter != this->connections.end()) {
        if ((*iter)->getCloseRequestStatus()) { // This connection was closed, flag is set -> remove its corresponding object and free the memory
            std::cout << "Connection with Id " << (*iter)->getConnectionId() << " closed! " << std::endl;
            this->connections.erase(iter); // Delete it from our vector
            if (this->connections.empty() || (iter == this->connections.end())) {
                return; // Don't increment the iterator when there is nothing to iterate over - avoids crash
            }
        } else {
            int currentFD = (*iter)->getFD();
            if (currentFD != 0) {
                FD_SET(currentFD, &(this->socks)); // Adds the socket file descriptor to the monitoring for select
                if (currentFD > this->highSock) {
                    this->highSock = currentFD; // We need the highest socket for select
                }
            }
        }

        ++iter; // Increment iterator
    }
}

// Accepts new connections and stores the connection object with fd in a vector
int servercore::handleNewConnection() {
    try {
        Socket newSocket = this->listenSocket.accept();

        // Gets the socket fd flags and add the non-blocking flag to the sfd
        newSocket.makeNonBlocking();

        // Get the client IP address
        char ipstr[INET6_ADDRSTRLEN];
        int port;
        this->addrLength = sizeof this->addrStorage;
        getpeername(newSocket.native(), (struct sockaddr*) &this->addrStorage, &(this->addrLength));
        std::string hostId = "";
        if (this->addr.sin_family == AF_INET) {
            struct sockaddr_in* fd = (struct sockaddr_in*) &(this->addrStorage);
            port = ntohs(fd->sin_port);
            inet_ntop(AF_INET, &fd->sin_addr, ipstr, sizeof ipstr);
            hostId = (std::string)ipstr;
        }

        std::cout << "Connection accepted: FD=" << newSocket.native()
            << " - Slot=" << (this->connections.size() + 1) 
            << (++this->connId) << std::endl;
        // The new connection (object)
        auto conn = std::make_unique<serverconnection>(
                std::move(newSocket), this->connId, this->dir, hostId, this->commandOffset); // The connection vector
        this->connections.push_back(std::move(conn));
    } catch (const std::exception& e) {
        std::cerr << "Error while accepting client" << std::endl;
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

// Something is happening (=data ready to read) at a socket, either accept a new connection or handle the incoming data over an already opened socket
void servercore::readSockets() {
    // OK, now socks will be set with whatever socket(s) are ready for reading. First check our "listening" socket, and then check the sockets in connectlist
    // If a client is trying to connect() to our listening socket, select() will consider that as the socket being 'readable' and thus, if the listening socket is part of the fd_set, accept a new connection
    if (FD_ISSET(this->listenSocket.native(), &(this->socks))) {
        if (handleNewConnection()) {
            return; // Always check for errors
        }
    }

    // Now check connectlist for available data
    // Run through our sockets and check to see if anything happened with them, if so 'service' them
    for (auto& connections : this->connections) {
        if (FD_ISSET(connections->getFD(), &(this->socks))) {
            connections->respondToQuery(); // Now that data is available, deal with it!
        }
    }
}

// Server entry point and main loop accepting and handling connections
int servercore::start() {
    struct timeval timeout; // Timeout for select
    timeout.tv_sec = 1; // Timeout = 1 sec
    timeout.tv_usec = 0;
    // Wait for connections, main server loop
    while (!this->shutdown) {

        this->buildSelectList(); // Clear out data handled in the previous iteration, clear closed sockets

        // Multiplexes between the existing connections regarding to data waiting to be processed on that connection (that's actually what select does)
        int readsocks = api()->select(this->highSock+1, &(this->socks), (fd_set*)0, (fd_set*)0, &timeout);
        if (readsocks < 0) {
            std::cerr << "Error calling select" << std::endl;
            return (EXIT_FAILURE);
        }

        readSockets(); // Handle the sockets (accept new connections or handle incoming data or do nothing [if no data])
    }

    return (EXIT_SUCCESS);
}

// Initialization of sockets / socket list with options and error checking
void servercore::initSockets(int port) {
    // PF_INET: domain, Internet; SOCK_STREAM: datastream, TCP / SOCK_DGRAM = UDP => WARNING, this can change the byte order!; for 3rd parameter==0: TCP preferred
    try {
        this->listenSocket = Socket(api()->socket(PF_INET, SOCK_STREAM, 0));
        if (this->listenSocket.allowReuse() < 0) {
            throw std::runtime_error("setsockopt() failed");
        }
        this->listenSocket.makeNonBlocking();

        this->addr.sin_family = AF_INET; // PF_INET;
        this->addr.sin_port = htons(port);
        this->addr.sin_addr.s_addr = INADDR_ANY; // Server can be connected to from any host
        this->listenSocket.bind(this->addr);

        this->maxConnectionsInQuery = 50;
        this->listenSocket.listen(this->maxConnectionsInQuery);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        shutdownServer();
        return;
    }

    this->highSock = this->listenSocket.native(); // This is the first (and the main listening) socket
    std::cout << "Server started and listening at port " << port << ", default server directory '" << this->dir << "'" << ((this->commandOffset == 3) ? ", for use with telnet" : "")  << std::endl;
}
