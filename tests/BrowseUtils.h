#pragma once

#include <dirent.h>
#include <string>

namespace BrowseUtils {

struct dirent makeDirEntry(const std::string& name, uint8_t type);

}
