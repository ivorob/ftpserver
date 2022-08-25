#include "OSApi.h"
#include "OSApiImpl.h"

namespace {

OSApiHolder impl = std::make_shared<OSApiImpl>();

}

OSApiHolder api()
{
    return impl;
}
