#pragma once

#include <iostream>
#include <bitset>

// A simple type alias
using ComponentType = std::uint8_t;

// Used to define the size of arrays later on
const ComponentType MAX_COMPONENTS = 32;

// A simple type alias
using Signature = std::bitset<MAX_COMPONENTS>;

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 100;