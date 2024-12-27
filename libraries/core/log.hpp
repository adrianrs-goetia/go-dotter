#pragma once

#include "godotIncludes.hpp"
#include <cstdint>
#include <iostream>
#include <source_location>

////////////////////////////////////////////////////
/// IMPLEMENTATION ///
////////////////////////////////////////////////////
enum class ELog : uint8_t {
	DEBUG,
	INFO,
	WARN,
	ERROR
};
static ELog g_loglevel = ELog::DEBUG;

constexpr const char* color_default = "0";
constexpr const char* color_fg_gray = "90";
constexpr const char* color_fg_green = "32";
constexpr const char* color_fg_yellow = "33";
constexpr const char* color_fg_red = "31";

inline const char* color_from_level(ELog level) {
	switch (level) {
		case ELog::DEBUG:
			return color_fg_gray;
		case ELog::INFO:
			return color_fg_green;
		case ELog::WARN:
			return color_fg_yellow;
		case ELog::ERROR:
			return color_fg_red;
		default:
			return "";
	}
}
inline const char* getString(ELog level) {
	switch (level) {
		case ELog::DEBUG:
			return "DEBUG";
		case ELog::INFO:
			return "INFO";
		case ELog::WARN:
			return "WARN";
		case ELog::ERROR:
			return "ERROR";
		default:
			return "";
	}
}

inline void __log_impl(ELog level, const char* msg, const char* arg) {
	if (level >= g_loglevel)
		::printf("\033[%sm %s %s \n\033[%sm", color_from_level(level), msg, arg, color_default);
}
inline void __log(ELog level, const char* msg) {
	__log_impl(level, msg, "");
}

inline std::string __toString(auto t) {
	using T = std::decay_t<decltype(t)>;
	if constexpr (std::is_same_v<T, const char*>) {
		return std::string(t);
	}
	if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, float> || std::is_same_v<T, double>
				  || std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t> || std::is_same_v<T, int32_t>
				  || std::is_same_v<T, int64_t> || std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>
				  || std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>) {
		return std::to_string(t);
	}
	else if constexpr (std::is_same_v<T, godot::String>) {
		return std::string(t.utf8().get_data());
	}
	else if constexpr (std::is_same_v<T, godot::StringName> || std::is_same_v<T, godot::NodePath>
					   || std::is_same_v<T, godot::Variant> || std::is_same_v<T, godot::Vector2>
					   || std::is_same_v<T, godot::Vector2i> || std::is_same_v<T, godot::Vector3>
					   || std::is_same_v<T, godot::Vector3i> || std::is_same_v<T, godot::Quaternion>
					   || std::is_same_v<T, godot::Transform3D>) {
		return std::string(godot::String(t).utf8().get_data());
	}
}

template <typename... Args>
inline std::string __getLogMsg(Args... args) {
	std::stringstream ss;
	(ss << ... << __toString(args));
	return ss.str();
}

////////////////////////////////////////////////////
/// API ///
////////////////////////////////////////////////////
#define LOG(level, ...)                                                                                                \
	{                                                                                                                  \
		std::stringstream ss;                                                                                          \
		auto s = std::source_location::current();                                                                      \
		ss << s.file_name() << ":" << s.line() << "	";                                                                 \
		ss << __getLogMsg(__VA_ARGS__);                                                                                \
		__log(ELog::level, ss.str().c_str());                                                                          \
	}

inline void setLogLevel(ELog level) {
	g_loglevel = ELog::DEBUG;
	LOG(DEBUG, "Setting log level -> ", getString(level));
	g_loglevel = level;
}
