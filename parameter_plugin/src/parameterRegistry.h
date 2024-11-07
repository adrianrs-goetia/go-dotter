#pragma once

#include <core/core.hpp>

#include <map>

template <typename T>
struct ParamRegistryEntry{
    std::vector<std::string> location;
    T value;
    T defaultValue;
};

class ParameterRegistry{
    //
    
public:

};