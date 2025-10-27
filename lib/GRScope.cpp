#include "GRScope.h"

#include <iostream>

bool initialized = false;
std::shared_ptr<J270SDR> instance = nullptr;

std::shared_ptr<J270SDR> init() {
    if (initialized)
        return instance;
    instance = J270SDR::autoScan();
    if (instance == nullptr)
        std::cerr << "Failed to initialize J270SDR instance, GNURadio may not work properly." << std::endl;
    initialized = true;
    return instance;
}