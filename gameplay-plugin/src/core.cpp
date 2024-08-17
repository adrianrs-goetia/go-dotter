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

ELog g_loglevel = ELog::DEBUG;

constexpr const char* color_default = "0";
constexpr const char* color_fg_gray = "37";
constexpr const char* color_fg_green = "32";
constexpr const char* color_fg_yellow = "33";
constexpr const char* color_fg_red = "31";

const char* color_from_level(ELog level) {
	switch (level) {
		case ELog::DEBUG: return color_fg_gray;
		case ELog::INFO: return color_fg_green;
		case ELog::WARN: return color_fg_yellow;
		case ELog::ERROR: return color_fg_red;
		default: return "";
	}
}
void Log(ELog level, const char* msg) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s \n\033[%sm", color_from_level(level), msg, color_default);
}
void Log(ELog level, const char* msg, const godot::Vector2 v2) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, godot::String(v2).utf8().get_data(),
				color_default);
}
void Log(ELog level, const char* msg, const godot::Vector3 v3) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, godot::String(v3).utf8().get_data(),
				color_default);
}
