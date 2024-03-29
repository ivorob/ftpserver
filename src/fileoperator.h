#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <memory>
#include <list>
#include <stdint.h>

#include "os.h"

// Buffer size
#define BUFFER_SIZE 4096

// This contains the designation for the server root directory
#define SERVERROOTPATHSTRING "<root>/"

/* The strict parameter distincts the access rights:
 * strict = true:
 *  Only file and directory names in the current working dir are permitted as parameter,
 *  especially references over several directories like ../../filename are prohibited
 *  to ensure we do not drop under the server root directory by user command,
 *  used with client side commands
 * strict = false:
 *  References over several directories like ../../filename are allowed as parameters,
 *  only used at server side
 */
class fileoperator {
public:
    fileoperator(std::string dir);
    virtual ~fileoperator();

    std::ifstream readFile(std::string fileName);
    std::unique_ptr<char[]>  readFileBlock(std::ifstream& input, unsigned long &sizeInBytes);
    int beginWriteFile(std::string fileName);
    int writeFileBlock(std::string content);
    void closeWriteFile();
    bool changeDir(std::string newPath, bool strict = true);
    std::string getCurrentWorkingDir(bool showRootPath = true);
    bool createFile(std::string &fileName, bool strict = true);
    bool createDirectory(std::string &dirName, bool strict = true);
    bool deleteFile(std::string fileName, bool strict = true);
    std::string getParentDir();
    void clearListOfDeletedFiles();
    void clearListOfDeletedDirectories();
    std::vector<std::string> getListOfDeletedFiles();
    std::vector<std::string> getListOfDeletedDirectories();
    bool dirIsBelowServerRoot(std::string dirName);
private:
    void getValidDir(std::string &dirName);
    void getValidFile(std::string &fileName);
    void stripServerRootString(std::string &dirOrFileName);
private:
    static void IntToString(int i, std::string &res);
private:
    std::vector<std::string> deletedDirectories;
    std::vector<std::string> deletedFiles;
    std::ofstream currentOpenFile;
    std::list<std::string> completePath; // The path from server root dir upwards to the current working dir, each list element containing one dir
};
