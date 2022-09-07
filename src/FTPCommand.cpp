#include "FTPCommand.h"

FTP::Command::Command(Context context)
    : context(std::move(context)) {
}

FTP::Context& FTP::Command::getContext() {
    return this->context;
}
