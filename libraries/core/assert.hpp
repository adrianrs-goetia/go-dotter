#pragma once

#include <iostream>
#include <sstream>

inline void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
	if (!expr) {
		std::cerr << "Assert failed:\t" << msg << "\n"
				  << "Expected:\t" << expr_str << "\n"
				  << "Source:\n"
				  << file << "\nline " << line << "\n";
		abort();
	}
}

// Have to be kept as std::string for now, returning const char* returns a ptr to the buffer
// but that is freed once stream goes out of scope.
// A little ugly to have this be at runtime and not something we could have purely at compile time
template <typename... Args>
std::string __concatenate_args(Args&&... args) {
	std::ostringstream oss;
	(oss << ... << args);
	return oss.str();
}

#define ASSERTM(expr, ...)                                                                                             \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, __concatenate_args(__VA_ARGS__).c_str()); }

#define ASSERT(expr)                                                                                                   \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, ""); }
#define ASSERT_MSG(expr, msg)                                                                                          \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, msg); }
#define ASSERT_NOTNULL(ptr) ASSERT(ptr != nullptr)
#define ASSERT_NOTNULL_MSG(ptr, msg) ASSERT_MSG(ptr != nullptr, msg)
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);
