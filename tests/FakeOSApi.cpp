#include "OSApi.h"

namespace {

OSApiHolder impl;

}

void setImpl(OSApiHolder newImpl)
{
    impl = newImpl;
}

OSApiHolder api()
{
    return impl;
}
