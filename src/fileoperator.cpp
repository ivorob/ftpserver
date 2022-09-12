#include "fileoperator.h"
#include "OSApi.h"

// Constructor, gets the current server root dir as a parameter
fileoperator::fileoperator(std::string dir) {
    this->completePath.push_front(std::move(dir)); // Insert the root dir at the beginning
}

// Destructor, let the cleaner rage
fileoperator::~fileoperator() {
    this->closeWriteFile(); // Close the file (if any is open)
}

// Relative directories only, strict
bool fileoperator::changeDir(std::string newPath, bool strict) {
    if (strict) {
        getValidDir(newPath); // check error cases, e.g. newPath = '..//' , '/home/user/' , 'subdir' (without trailing slash), etc...
    }

    // If change to a higher directory is requested
    if ( (newPath.compare("..") == 0) || (newPath.compare("../") == 0) ) {
        // If we are already in the server root dir, prohibit the change to a higher directory
        if (this->completePath.size() <= 1) {
            std::cerr << "Error: Change beyond server root requested (prohibited)!" << std::endl;
            return (EXIT_FAILURE); // 1
        } else { // change is permitted, now do it!
            this->completePath.pop_back(); // Remove the last dir we were in and return to the lower one
            return (EXIT_SUCCESS); // 0
        }
    }
    // The change is the local directory !?
    if ( (newPath.compare(".") == 0) || (newPath.compare("./") == 0)) {
        std::cout << "Change to same dir requested (nothing done)!" << std::endl;
        return (EXIT_SUCCESS); // 0 (?)
    }

    // Normal (sub-)directory given
    if (api()->canOpenDirectory(this->getCurrentWorkingDir().append(newPath))) {
        this->completePath.push_back(newPath); // Add the new working dir to our path list
        return (EXIT_SUCCESS); // 0
    } else {
        return (EXIT_FAILURE); // 1
    }
}

// check error cases, e.g. newPath = '..//' , '/home/user/' , 'subdir' (without trailing slash), etc... and return a clean, valid string in the form 'subdir/'
void fileoperator::getValidDir(std::string& dirName) {
    std::string slash = "/";
    size_t foundSlash = 0;
    while ((foundSlash = dirName.find_first_of(slash), (foundSlash)) != std::string::npos) {
        dirName.erase(foundSlash++, 1); // Remove all slashs
    }

    dirName.append(slash); // Trailing slash is good and required, append it
}

//filename ../../e.txt -> e.txt , prohibit deletion out of valid environment
void fileoperator::getValidFile(std::string &fileName) {
    std::string slash = "/";
    size_t foundSlash = 0;
    while ((foundSlash = fileName.find_first_of(slash), (foundSlash)) != std::string::npos) {
        fileName.erase(foundSlash++, 1); // Remove all slashs
    }
}

// Strips out the string for the server root, e.g. <root>/data/file -> data/file
void fileoperator::stripServerRootString(std::string &dirOrFileName) {
    size_t foundRootString = 0;
    if ((dirOrFileName.find_first_of(SERVERROOTPATHSTRING)) == foundRootString) {
        size_t rootStringLength = ((std::string)SERVERROOTPATHSTRING).length();
        dirOrFileName = dirOrFileName.substr(rootStringLength, dirOrFileName.length() - rootStringLength);
    }
}

// Creates a directory with the specified name in the current working directory
bool fileoperator::createDirectory(std::string &dirName, bool strict) {
    if (strict) {
        getValidDir(dirName); // check error cases, e.g. newPath = '..//' , '/home/user/' , 'subdir' (without trailing slash), etc...
    }

    // Prohibit deletion of dir ../ beyond server root
    if ((dirName.compare("../") == 0) && (this->completePath.size() < 2)) {
        dirName = "./";
        std::cerr << "Error: Deletion of dir beyond server root requested (prohibited)!" << std::endl;
        return true;
    }

    umask(0);
    return ((api()->mkdir(this->getCurrentWorkingDir().append(dirName).c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

// Read a block from the open file
std::unique_ptr<char[]> fileoperator::readFileBlock(std::ifstream& input, unsigned long& sizeInBytes) {
    sizeInBytes = 0;
    if (!input) {
        return {};
    }

    // get length of file
    input.seekg(0, std::ios::end);
    std::ifstream::pos_type size = input.tellg();
    sizeInBytes = (unsigned long)size;
    if (size == 0) {
        std::cout << "Try to read empty file!" << std::endl;
        return {};
    }

    input.seekg(0, std::ios::beg);
    // allocate memory
    std::unique_ptr<char[]> memblock(new char[size]);
    // read data as a block
    input.read(memblock.get(), size);

    std::cout << "Reading " << size << " Bytes" << std::endl;
    return memblock;
}

/// @WARNING, @KLUDGE: Concurrent file access not catched
std::ifstream fileoperator::readFile(std::string fileName) {
    stripServerRootString(fileName);

    std::ifstream input(fileName, std::ios::in | std::ios::binary); // modes for binary file  |std::ios::ate
    if (input.fail()) {
        std::cout << "Reading file '" << fileName << "' failed!" << std::endl; //  strerror(errno) <<
        return input;
    }

    if (input.is_open()) {
        return input;
    }

    std::cerr << "Unable to open file '" << fileName << " '" << std::endl; // << strerror(errno)
    return input;
}

int fileoperator::beginWriteFile(std::string fileName) {
    stripServerRootString(fileName);

    this->currentOpenFile.open(fileName.c_str(), std::ios::out|std::ios::binary|std::ios::app); // output file
    if(!this->currentOpenFile) {
        std::cerr << "Cannot open output file '" << fileName << "'" << std::endl;
        return (EXIT_FAILURE);
    }
    std::cout << "Beginning writing to file '" << fileName << "'" << std::endl;
    return (EXIT_SUCCESS);
}

/// @WARNING, @KLUDGE: Concurrent file access not catched
int fileoperator::writeFileBlock(std::string content) {
    if (!this->currentOpenFile.is_open()) {
        std::cerr << "Cannot write to output file" << std::endl;
        return (EXIT_FAILURE);
    }

    std::cout << "Appending to file" << std::endl;
    (this->currentOpenFile) << content;
    return (EXIT_SUCCESS);
}

// File is closed when disconnecting
void fileoperator::closeWriteFile() {
    if (this->currentOpenFile.is_open()) {
        std::cout << "Closing open file" << std::endl;
        this->currentOpenFile.close();
    }
}

// Same as unix touch command
// Avoid touch ../file beyond server root!
bool fileoperator::createFile(std::string &fileName, bool strict) {
    if (strict) {
        this->getValidFile(fileName); // Avoid touch ../file beyond server root!
    }

    try {
        std::ofstream fileout;
        fileout.open(this->getCurrentWorkingDir().append(fileName).c_str(), std::ios::out|std::ios::binary|std::ios::app);
        fileout.close();
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

/*
 * Deletes a file on the server given by its name
 *
 * @param fileName the name of the file to delete
 */
// Avoid rm ../file beyond server root!
bool fileoperator::deleteFile(std::string fileName, bool strict) {
    if (strict) {
        this->getValidFile(fileName); // Avoid rm ../file beyond server root!
    }

    if (api()->remove(this->getCurrentWorkingDir().append(fileName).c_str()) != 0 ) {
        std::cerr << "Error deleting file '" << fileName << "'" << std::endl;
        return (EXIT_FAILURE);
    } else {
        std::cout << "File '" << fileName << "' deleted" << std::endl;
        this->deletedFiles.push_back(fileName);
        return (EXIT_SUCCESS);
    }
}

void fileoperator::IntToString(int i, std::string& res) {
    std::ostringstream temp;
    temp << i;
    res = temp.str();
}

/*
 * Returns the directory the given file resides in (only directory and no full path, required?)
 */
std::string fileoperator::getParentDir() {
    std::list<std::string>::reverse_iterator lastDirs;
    unsigned int i = 0;
    // Walk to second-last entry for parent dir (from end of list on)
    for (lastDirs = this->completePath.rbegin(); ((lastDirs != this->completePath.rend()) && (++i <= 2)); ++lastDirs) {
        if (i == 2) { // Yes this is the second-last entry (could also be that only one dir exists because we are in the server root dir
            return ((++lastDirs == this->completePath.rend()) ? SERVERROOTPATHSTRING : (*(--lastDirs))); // Avoid divulging the real server root path
        }
    }
    return SERVERROOTPATHSTRING; // Otherwise, the server root dir is the parent directory (e.g. "./")
//    return this->completePath.front(); // Otherwise, the server root dir is the parent directory (e.g. "./")
}

// Returns the path to the current working dir starting from the server root dir
std::string fileoperator::getCurrentWorkingDir(bool showRootPath) {
    std::string fullpath = "";
    std::list<std::string>::iterator singleDir;
    for (singleDir = this->completePath.begin(); singleDir != this->completePath.end(); ++singleDir) {
        // If first entry (= server root)
        if ( singleDir == this->completePath.begin() ) {
            // If the real root path should be hidden to client
            if (!showRootPath) {
                fullpath.append(SERVERROOTPATHSTRING);                
            }
        } else {
            fullpath.append(*(singleDir));
        }
    }

    return fullpath;
}

// Clears out the list of deleted files
void fileoperator::clearListOfDeletedFiles() {
    this->deletedFiles.clear();
}

// Clears out the list of deleted directories
void fileoperator::clearListOfDeletedDirectories() {
    this->deletedDirectories.clear();
}

// Returns the list of successfully deleted files
std::vector<std::string> fileoperator::getListOfDeletedFiles() {
    return this->deletedFiles;
}

// Returns the list of successfully deleted directories
std::vector<std::string> fileoperator::getListOfDeletedDirectories() {
    return this->deletedDirectories;
}

// Check if the specified directory is below the server root directory and return true if so
bool fileoperator::dirIsBelowServerRoot(std::string dirName) {
    this->getValidDir(dirName);
    return ((dirName.compare("../") == 0) && (this->completePath.size() < 2));
}
