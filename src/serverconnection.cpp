#include <iterator>

#include "serverconnection.h"
#include "OSApi.h"

// Constructor
serverconnection::serverconnection(Socket currentSocket, unsigned int connId, std::string defaultDir, std::string hostId, unsigned short commandOffset) 
    : fo(std::make_shared<fileoperator>(dir))
    , currentSocket(std::move(currentSocket))
    , ftpCommandFactory(makeContext())
    , connectionId(connId)
    , dir(defaultDir)
    , hostAddress(hostId)
    , commandOffset(commandOffset)
    , closureRequested(false)
    , uploadCommand(false)
    , downloadCommand(false)
    , receivedPart(0)
    , parameter("") {

    // Send hello
    std::string data = "220 FTP server ready.\n";
    sendToClient(data.c_str(), data.size());
    std::cout << "Connection to client '" << this->hostAddress << "' established" << std::endl;
}

FTP::Context serverconnection::makeContext() {
    FTP::Context ftpContext;
    ftpContext.shutdownConnection = std::bind(&serverconnection::shutdown, this);
    ftpContext.fileoperator = this->fo;
    return ftpContext;
}

void serverconnection::shutdown() {
    this->closureRequested = true;
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
    this->uploadCommand = false;
    struct stat Status;
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
    /*if (commandAndParameter.size() == 1) {
        if (this->commandEquals(commandAndParameter.at(0), "list")) {
            // dir to browse
            std::string curDir = "./";
            std::cout << "Browsing files of the current working dir" << std::endl;
            this->directories.clear();
            this->files.clear();
            this->fo->browse(curDir,directories,files);
            for (unsigned int j = 0; j < directories.size(); j++) {
                res += directories.at(j) + "/\n";
            }
            for (unsigned int i = 0; i < files.size(); i++) {
                res += files.at(i) + "\n";
            }
        } else if (this->commandEquals(commandAndParameter.at(0), "pwd")) { // Returns the current working directory on the server
            std::cout << "Working dir requested" << std::endl;
            res = "257 \"" + this->fo->getCurrentWorkingDir(false) + "\"";
        } else if (this->commandEquals(commandAndParameter.at(0), "getparentdir")) { // Returns the parent directory of the current working directory on the server
            std::cout << "Parent dir of working dir requested" << std::endl;
            res = this->fo->getParentDir();
        } else if (this->commandEquals(commandAndParameter.at(0), "bye") || this->commandEquals(commandAndParameter.at(0), "quit")) {
            std::cout << "Shutdown of connection requested" << std::endl;
            this->closureRequested = true;
            res = "221 Goodbye.";
        } else {
            // Unknown / no command / enter
            std::cout << "Unknown command encountered '" << commandAndParameter.at(0) << "'!" << std::endl;
            commandAndParameter.clear();
        }
    } else if (commandAndParameter.size() > 1) { // Command with a parameter received
        // The parameter
        this->parameter = commandAndParameter.at(1);        
        if (this->commandEquals(commandAndParameter.at(0), "ls")) {
            // read out dir to browse
            std::string curDir = std::string(commandAndParameter.at(1));
            std::cout << "Browsing files of directory '" << curDir << "'" << std::endl;
            this->directories.clear();
            this->files.clear();
            this->fo->browse(curDir, directories, files);
            for (unsigned int j = 0; j < directories.size(); j++) {
                res += directories.at(j) + "/\n";
            }
            for (unsigned int i = 0; i < files.size(); i++) {
                res += files.at(i) + "\n";
            }
        } else if (this->commandEquals(commandAndParameter.at(0), "download")) {
            this->downloadCommand = true;
            std::cout << "Preparing download of file '" << this->parameter << "'" << std::endl;
            unsigned long lengthInBytes = 0;
            std::ifstream input = this->fo->readFile(this->parameter);
            if (input) {
                auto fileBlock = this->fo->readFileBlock(input, lengthInBytes);
                this->sendToClient(fileBlock.get(), lengthInBytes); // This sends the binary char-array to the client
            }
            this->closureRequested = true; // Close connection after transfer
        } else if (this->commandEquals(commandAndParameter.at(0), "upload")) {
            this->uploadCommand = true; // upload hit!
            std::cout << "Preparing download of file '" << this->parameter << "'" << std::endl;
            // all bytes (=parameters[2]) after the upload <file> command belong to the file
            res = this->fo->beginWriteFile(this->parameter);
        } else if (this->commandEquals(commandAndParameter.at(0), "cwd")) { // Changes the current working directory on the server
            std::cout << "Change of working dir to '" << this->parameter << "' requested" << std::endl;
            // Test if dir exists
            if (!this->fo->changeDir(this->parameter)) {
                std::cout << "Directory change to '" << this->parameter << "' successful!" << std::endl;
            }
            res = "250 directory changed to " + this->fo->getCurrentWorkingDir(false);
        } else if (this->commandEquals(commandAndParameter.at(0), "rmdir")) {
            std::cout << "Deletion of dir '" << this->parameter << "' requested" << std::endl;
            if (this->fo->dirIsBelowServerRoot(this->parameter)) {
                std::cerr << "Attempt to delete directory beyond server root (prohibited)" << std::endl;
                res = "//"; // Return some garbage as error indication :)
            } else {
                this->directories.clear(); // Reuse directories to spare memory
                this->fo->clearListOfDeletedDirectories();
                this->files.clear(); // Reuse files to spare memory
                this->fo->clearListOfDeletedFiles();
                if (this->fo->deleteDirectory(this->parameter)) {
                    std::cerr << "Error when trying to delete directory '" << this->parameter << "'" << std::endl;
                }
                this->directories = this->fo->getListOfDeletedDirectories();
                this->files = this->fo->getListOfDeletedFiles();
                for (unsigned int j = 0; j < directories.size(); j++) {
                    res += directories.at(j) + "\n";
                }
                for (unsigned int i = 0; i < files.size(); i++) {
                    res += files.at(i) + "\n";
                }
            }
        } else if (this->commandEquals(commandAndParameter.at(0), "delete")) {
            std::cout << "Deletion of file '" << this->parameter << "' requested" << std::endl;
            this->fo->clearListOfDeletedFiles();
            if (this->fo->deleteFile(this->parameter)) {
                res = "//";
            } else {
                std::vector<std::string> deletedFile = this->fo->getListOfDeletedFiles();
                if (deletedFile.size() > 0)
                    res = deletedFile.at(0);
            }
        } else if (this->commandEquals(commandAndParameter.at(0), "getsize")) {
            std::cout << "Size of file '" << this->parameter << "' requested" << std::endl;
            std::vector<std::string> fileStats = this->fo->getStats(this->parameter, Status);
            res = fileStats.at(4); // file size or content count of directory
        } else if (this->commandEquals(commandAndParameter.at(0), "getaccessright")) {
            std::cout << "Access rights of file '" << this->parameter << "' requested" << std::endl;
            std::vector<std::string> fileStats = this->fo->getStats(this->parameter, Status);
            res = fileStats.at(0); // unix file / directory permissions
        } else if (this->commandEquals(commandAndParameter.at(0), "getlastmodificationtime")) {
            std::cout << "Last modification time of file '" << this->parameter << "' requested" << std::endl;
            std::vector<std::string> fileStats = this->fo->getStats(this->parameter, Status);
            res = fileStats.at(3); // unix file / directory permissions
        } else if (this->commandEquals(commandAndParameter.at(0), "getowner")) {
            std::cout << "Owner of file '" << this->parameter << "' requested" << std::endl;
            std::vector<std::string> fileStats = this->fo->getStats(this->parameter, Status);
            res = fileStats.at(2); // owner
        } else if (this->commandEquals(commandAndParameter.at(0), "getgroup")) {
            std::cout << "Group of file '" << this->parameter << "' requested" << std::endl;
            std::vector<std::string> fileStats = this->fo->getStats(this->parameter, Status);
            res = fileStats.at(1); // group
        } else if (this->commandEquals(commandAndParameter.at(0), "mkdir")) { // Creates a directory of the specified name in the current server working dir
            std::cout << "Creating of dir '" << this->parameter << "' requested" << std::endl;
            res = (this->fo->createDirectory(this->parameter) ? "//" : this->parameter); // return "//" in case of failure
        } else if (this->commandEquals(commandAndParameter.at(0), "touch")) { // Creates an empty file of the specified name in the current server working dir
            std::cout << "Creating of empty file '" << this->parameter << "' requested" << std::endl;
            res = (this->fo->createFile(this->parameter) ? "//" : this->parameter);  // return "//" in case of failure
        } else if (this->commandEquals(commandAndParameter.at(0), "user")) {
            std::cout << "User '" << this->parameter << "' is logged successfully!" << std::endl;
            res = "230 User logged in, proceed.";
        } else {
            // Unknown / no command
            std::cout << "Unknown command encountered '" << commandAndParameter.at(0) << "'!" << std::endl;
            res = "502 " + ftpCommand->execute(
                {std::make_move_iterator(commandAndParameter.begin() + 1),
                 std::make_move_iterator(commandAndParameter.end())}
            );

            commandAndParameter.clear();
            command = "";
        }
    }*/

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
        if (this->uploadCommand) { // (Previous) upload command
            /// Previous (upload) command continuation, store incoming data to the file
            std::cout << "Part " << ++(this->receivedPart) << ": ";
            this->fo->writeFileBlock(clientCommand);
        } else {
            // If not upload command issued, parse the incoming data for command and parameters
            std::string res = this->commandParser(clientCommand);
            this->sendToClient(res); // Send response to client if no binary file
        }
    } else { // no bytes incoming over this connection
        if (this->uploadCommand) { // If upload command was issued previously and no data is left to receive, close the file and connection
            this->uploadCommand = false;
            this->downloadCommand = false;
            this->closureRequested = true;
            this->receivedPart = 0;
        }
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
int serverconnection::getFD() const {
    return this->currentSocket.native();
}

// Returns whether the connection was requested to be closed (by client)
bool serverconnection::getCloseRequestStatus() const {
    return this->closureRequested;
}

unsigned int serverconnection::getConnectionId() const {
    return this->connectionId;
}
