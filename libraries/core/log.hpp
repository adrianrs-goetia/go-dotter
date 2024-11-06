#pragma once

#include "godotIncludes.hpp"
#include <cstdint>
#include <iostream>

////////////////////////////////////////////////////
/// IMPLEMENTATION ///
////////////////////////////////////////////////////
enum class ELog : uint8_t { DEBUG, INFO, WARN, ERROR };
static ELog g_loglevel = ELog::DEBUG;

constexpr const char* color_default = "0";
constexpr const char* color_fg_gray = "90";
constexpr const char* color_fg_green = "32";
constexpr const char* color_fg_yellow = "33";
constexpr const char* color_fg_red = "31";

inline const char* color_from_level(ELog level) {
	switch (level) {
		case ELog::DEBUG: return color_fg_gray;
		case ELog::INFO: return color_fg_green;
		case ELog::WARN: return color_fg_yellow;
		case ELog::ERROR: return color_fg_red;
		default: return "";
	}
}
inline const char* getString(ELog level) {
	switch (level) {
		case ELog::DEBUG: return "DEBUG";
		case ELog::INFO: return "INFO";
		case ELog::WARN: return "WARN";
		case ELog::ERROR: return "ERROR";
		default: return "";
	}
}

inline void log_impl(ELog level, const char* msg, const char* arg) {
	if (level >= g_loglevel) ::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, arg, color_default);
}
inline void Log(ELog level, const char* msg) { log_impl(level, msg, ""); }
inline void Log(ELog level, const char* msg, const char* arg) { log_impl(level, msg, arg); }
inline void Log(ELog level, const char* msg, const char* arg, const char* arg2) {
	std::string marg = std::string(arg) + std::string(arg2);
	log_impl(level, msg, marg.c_str());
}
inline void Log(ELog level, const char* msg, const bool arg) { log_impl(level, msg, arg ? "true" : "false"); }
inline void Log(ELog level, const char* msg, const float arg) { log_impl(level, msg, std::to_string(arg).c_str()); }
inline void Log(ELog level, const char* msg, const int64_t arg) { log_impl(level, msg, std::to_string(arg).c_str()); }
inline void Log(ELog level, const char* msg, const uint64_t arg) { log_impl(level, msg, std::to_string(arg).c_str()); }
inline void Log(ELog level, const char* msg, const godot::String& s) { log_impl(level, msg, s.utf8().get_data()); }
inline void Log(ELog level, const char* msg, const godot::Vector2& v2) {
	log_impl(level, msg, godot::String(v2).utf8().get_data());
}
inline void Log(ELog level, const char* msg, const godot::Vector3& v3) {
	log_impl(level, msg, godot::String(v3).utf8().get_data());
}
inline void Log(ELog level, const char* msg, const godot::Quaternion& q) {
	log_impl(level, msg, godot::String(q).utf8().get_data());
}
inline void Log(ELog level, const char* msg, const godot::Transform3D& t) {
	log_impl(level, msg, godot::String(t).utf8().get_data());
}


////////////////////////////////////////////////////
/// API ///
////////////////////////////////////////////////////
#define LOG(level, msg, ...)                                                                                           \
	{                                                                                                                  \
		const char* message = ("%s :: %s", __FUNCTION__, msg);                                                         \
		Log(ELog::level, message, ##__VA_ARGS__);                                                                      \
	}

inline void setLogLevel(ELog level) {
	g_loglevel = ELog::DEBUG;
	LOG(DEBUG, "Setting log level -> ", getString(level));
	g_loglevel = level;
}