#pragma once

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

#define ASSERT(expr)                                                                                                   \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, ""); }
#define ASSERT_MSG(expr, msg)                                                                                          \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, msg); }
#define ASSERT_NOTNULL(ptr) ASSERT(ptr != nullptr)
#define ASSERT_NOTNULL_MSG(ptr, msg) ASSERT_MSG(ptr != nullptr, msg)
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);
