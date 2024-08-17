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
constexpr const char* color_fg_gray = "90";
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
const char* get_string(ELog level) {
	switch (level) {
		case ELog::DEBUG: return "DEBUG";
		case ELog::INFO: return "INFO";
		case ELog::WARN: return "WARN";
		case ELog::ERROR: return "ERROR";
		default: return "";
	}
}
void set_log_level(ELog level) {
	g_loglevel = ELog::DEBUG;
	LOG(DEBUG, "Setting log level -> ", get_string(level));
	g_loglevel = level;
}
void Log(ELog level, const char* msg) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s \n\033[%sm", color_from_level(level), msg, color_default);
}
void Log(ELog level, const char* msg, const char* arg) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, arg, color_default);
}
void Log(ELog level, const char* msg, const float arg) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %f \n\033[%sm", color_from_level(level), msg, arg, color_default);
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
void Log(ELog level, const char* msg, const godot::Quaternion q) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, godot::String(q).utf8().get_data(),
				color_default);
}
void Log(ELog level, const char* msg, const godot::Transform3D t) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, godot::String(t).utf8().get_data(),
				color_default);
}