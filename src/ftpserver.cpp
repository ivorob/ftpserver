#include <cstdlib>
#include <iostream>
#include <memory>
#include <filesystem>

#include "servercore.h"
#include "OSApi.h"

namespace fs = std::filesystem;

/**
 * This is the main program entry point
 * usage:
 * ftpserver <server root directory> <port to listen on for incoming connections> <telnet mode for use with telnet client, default = false>
 */
int main(int argc, char** argv) {
    unsigned short commandOffset = 1; // For telnet, we need 3 because of the enter control sequence at the end of command (+2 characters)
    unsigned int port = 4242; // Port to listen on (>1024 for no root permissions required)
    std::string dir = "./"; // Default dir
    if (argc < 2) {
        std::cout << "Usage: ftpserver <dir> <port> [telnetmode=no], using default dir '" << dir << "' , port " << port << std::endl;
    } else {
        switch (argc) {
            case 4:
                commandOffset = 3; // If any 3rd parameter is given, the server is started for use with telnet as client
            case 3:
                port = atoi(argv[2]); // Cast str to int, set port
            case 2:
                // TODO: replace it by std::filesystem
                auto db = std::make_unique<fileoperator>(dir);
                // Test if dir exists
                if (api()->canOpenDirectory(argv[1])) {
                    dir = argv[1]; // set default server directory
                    fs::current_path(dir);
                } else {
                    std::cout << "Invalid path specified ('" << argv[1] << "'), falling back to '" << dir << "'" << std::endl;
                }
                break;
        }
    }

    auto myServer = std::make_unique<servercore>(port, dir, commandOffset);

    return (EXIT_SUCCESS);
}
