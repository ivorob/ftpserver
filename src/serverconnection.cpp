#include <iterator>

#include "serverconnection.h"
#include "OSApi.h"

// Constructor
serverconnection::serverconnection(Socket currentSocket, unsigned int connId, std::string defaultDir, std::string hostId, unsigned short commandOffset) 
    : fo(std::make_shared<fileoperator>(defaultDir))
    , currentSocket(std::move(currentSocket))
    , ftpCommandFactory(makeContext())
    , closureRequested(false)
    , connectionId(connId)
    , dir(std::move(defaultDir))
    , hostAddress(std::move(hostId))
    , commandOffset(commandOffset) {

    // Send hello
    sendToClient("220 FTP server ready.\n");
    std::cout << "Connection to client '" << this->hostAddress << "' established" << std::endl;
}

FTP::Context serverconnection::makeContext() {
    FTP::Context ftpContext;
    ftpContext.shutdownConnection = std::bind(&serverconnection::shutdown, this);
    ftpContext.obtainDataConnection = std::bind(&serverconnection::obtainDataConnection, this);
    ftpContext.fileOperator = this->fo;
    return ftpContext;
}

void serverconnection::shutdown() {
    this->closureRequested = true;
}

struct sockaddr_in serverconnection::obtainDataConnection() {
    return currentSocket.getAddress();
}

// Destructor, clean up all the mess
serverconnection::~serverconnection() {
    std::cout << "Connection terminated to client (connection id " << this->connectionId << ")" << std::endl;
}

// Check for matching (commands/strings) with compare method
bool serverconnection::commandEquals(std::string command, std::string commandToCompare) {
    // Convert to lower case for case-insensitive checking
    std::transform(command.begin(), command.end(), command.begin(), tolower);
    return (command.find(commandToCompare) != std::string::npos);
}

// Command switch for the issued client command, only called when this->command is set to 0
std::string serverconnection::commandParser(std::string command) {
    // Commands can have either 0 or 1 parameters, e.g. 'browse' or 'browse ./'
    std::vector<std::string> commandAndParameter = this->extractParameters(command);
    std::cout << "Connection " << this->connectionId << ": ";

    if (commandAndParameter.empty()) {
        return "\n";
    }

    /// @TODO: Test if prone to DOS-attacks (if loads of garbage is submitted)???
    // If command with no argument was issued
    auto ftpCommand = ftpCommandFactory.create(commandAndParameter.at(0));
    std::string res = ftpCommand->execute(
        {std::make_move_iterator(commandAndParameter.begin() + 1),
         std::make_move_iterator(commandAndParameter.end())}
    );

    res += "\n";
    return res;
}

// Extracts the command and parameter (if existent) from the client call
std::vector<std::string> serverconnection::extractParameters(std::string command) {
    while (!command.empty() && (command.back() == '\n' || command.back() == '\r')) {
        command.pop_back();
    }

    std::size_t previouspos = 0;
    std::size_t position = std::string::npos;
    std::vector<std::string> result;
    // First get the command by taking the string and walking from beginning to the first blank
    if ((position = command.find(SEPARATOR, previouspos)) != std::string::npos) { // No empty string
        result.push_back(command.substr(static_cast<int>(previouspos), static_cast<int>(position - previouspos))); // The command
    }

    if (command.length() > (position + 1)) {
        result.push_back(
            command.substr(
                static_cast<int>(position + 1),
                static_cast<int>(command.length() - (position + (this->commandOffset)))
            )
        ); // The parameter (if existent)
    }

    return result;
}

// Receives the incoming data and issues the apropraite commands and responds
void serverconnection::respondToQuery() {
    char buffer[BUFFER_SIZE] = {0};
    int bytes = api()->recv(this->currentSocket.native(), buffer, sizeof(buffer), 0);
    // In non-blocking mode, bytes <= 0 does not mean a connection closure!
    if (bytes > 0) {
        std::string clientCommand = std::string(buffer, bytes);

        std::string res = this->commandParser(clientCommand);
        this->sendToClient(res); // Send response to client if no binary file
    } else { // no bytes incoming over this connection
        this->closureRequested = true;
    }
}

// Sends the given string to the client using the current connection
void serverconnection::sendToClient(const char* response, unsigned long length) {
    // Now we're sending the response
    unsigned int bytesSend = 0;
    while (bytesSend < length) {
        int result = api()->send(this->currentSocket.native(), response + bytesSend, length - bytesSend, 0);
        if (result <= 0) {
            return;
        }

        bytesSend += result;
    }
}

// Sends the given string to the client using the current connection
void serverconnection::sendToClient(std::string response) {
    // Now we're sending the response
    unsigned int bytesSend = 0;
    while (bytesSend < response.length()) {
        int result = api()->send(this->currentSocket.native(), 
            response.c_str() + bytesSend, response.length() - bytesSend, 0);
        if (result <= 0) {
            return;
        }

        bytesSend += result;
    }
}

// Returns the file descriptor of the current connection
SOCKET serverconnection::getFD() const {
    return this->currentSocket.native();
}

// Returns whether the connection was requested to be closed (by client)
bool serverconnection::getCloseRequestStatus() const {
    return this->closureRequested;
}

unsigned int serverconnection::getConnectionId() const {
    return this->connectionId;
}
