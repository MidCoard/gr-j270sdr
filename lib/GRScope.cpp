#include "GRScope.h"

bool initialized = false;
std::shared_ptr<J270SDR> instance = nullptr;

std::shared_ptr<J270SDR> init() {
    if (initialized)
        return instance;
    instance = J270SDR::autoScan();
    initialized = true;
    return instance;
}