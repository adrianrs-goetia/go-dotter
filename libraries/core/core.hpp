#pragma once

#include "godotIncludes.hpp"
#include "godotUtils.hpp"
#include "log.hpp"
#include "math.hpp"
#include "timer.hpp"
#include "typedefs.hpp"
#include "utils.hpp"

#include <iostream>

inline void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
	if (!expr) {
		std::cerr << "Assert failed:\t" << msg << "\n"
				  << "Expected:\t" << expr_str << "\n"
				  << "Source:\n"
				  << file << "\nline " << line << "\n";
		abort();
	}
}
#define ASSERT(expr, msg)                                                                                              \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, msg); }
#define ASSERT_NOTNULL(ptr) ASSERT(ptr != nullptr, "")
#define ASSERT_NOTNULL_MSG(ptr, msg) ASSERT(ptr != nullptr, msg)
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);

#ifdef _WIN32
    // #ifdef LIBRARY_EXPORTS
        #define DD3D_API __declspec(dllexport)
    // #else
        // #define DD3D_API __declspec(dllimport)
    // #endif
#else
    #define API __attribute__((visibility("default")))
#endif