#include <core/core.h>
#include <iostream>
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
	if (!expr) {
		std::cerr << "Assert failed:\t" << msg << "\n"
				  << "Expected:\t" << expr_str << "\n"
				  << "Source:\n" << file << "\nline " << line << "\n";
		abort();
	}
}