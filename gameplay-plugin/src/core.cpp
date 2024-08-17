#include <core/core.h>
#include <iostream>
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
	if (!expr) {
		std::cerr << "Assert failed:\t" << msg << "\n"
				  << "Expected:\t" << expr_str << "\n"
				  << "Source:\n"
				  << file << "\nline " << line << "\n";
		abort();
	}
}

// fg bg
// foreground background
constexpr const char* color_default = "0";
constexpr const char* color_fg_gray = "37";
constexpr const char* color_fg_green = "32";
constexpr const char* color_fg_yellow = "33";
constexpr const char* color_fg_red = "31";

Log::Level level = Log::Level::INFO;

void Log::debug(const char* msg) {
	if (level <= Log::Level::DEBUG)
		::printf("\033[%sm %s \n\033[%sm", color_fg_gray, msg, color_default);
}
void Log::info(const char* msg) {
	if (level <= Log::Level::INFO)
		::printf("\033[%sm %s \n\033[%sm", color_fg_green, msg, color_default);
}

void Log::warn(const char* msg) {
	if (level <= Log::Level::WARN)
		::printf("\033[%sm %s \n\033[%sm", color_fg_yellow, msg, color_default);
}

void Log::error(const char* msg) {
	if (level <= Log::Level::ERROR)
		::printf("\033[%sm %s \n\033[%sm", color_fg_red, msg, color_default);
}