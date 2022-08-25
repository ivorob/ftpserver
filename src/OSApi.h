#pragma once

#include <memory>

#include "AbstractOSApi.h"

using OSApiHolder = std::shared_ptr<AbstractOSApi>;

OSApiHolder api();
