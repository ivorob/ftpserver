#include <string.h>

#include "BrowseUtils.h"

struct dirent BrowseUtils::makeDirEntry(const std::string& name, uint8_t type)
{
    struct dirent dirEntry = {0};
#if defined(__APPLE_CC__)
    dirEntry.d_ino = 0;
#else
    dirEntry.d_fileno = 0;
    dirEntry.d_off = 0;
#endif
    dirEntry.d_reclen = sizeof(struct dirent);
    dirEntry.d_type = type;
#if defined(__FreeBSD__) || defined(__APPLE_CC__)
    dirEntry.d_namlen = name.size();
#endif
    strncpy(dirEntry.d_name, name.c_str(), sizeof(dirEntry.d_name));
    return dirEntry;
}

