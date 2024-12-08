#pragma once

#include "assert.hpp"
#include "godotIncludes.hpp"
#include "godotUtils.hpp"
#include "log.hpp"
#include "math.hpp"
#include "timer.hpp"
#include "typedefs.hpp"
#include "utils.hpp"

#ifdef _WIN32
// #ifdef LIBRARY_EXPORTS
#define DD3D_API __declspec(dllexport)
// #else
// #define API __declspec(dllimport)
// #endif
#else
#define API __attribute__((visibility("default")))
#endif
