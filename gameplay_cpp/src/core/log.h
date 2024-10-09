#pragma once

#include "godotincludes.h"
#include <cstdint>

enum class ELog : uint8_t { DEBUG, INFO, WARN, ERROR };
#define LOG(level, msg, ...)                                                                                           \
	{                                                                                                                  \
		const char* message = ("%s :: %s", __FUNCTION__, msg);                                                         \
		Log(ELog::level, message, ##__VA_ARGS__);                                                                      \
	}

void set_log_level(ELog level);
const char* get_string(ELog level);
void Log(ELog level, const char* msg);
void Log(ELog level, const char* msg, const char* arg);
void Log(ELog level, const char* msg, const char* arg, const char* arg2);
void Log(ELog level, const char* msg, const bool arg);
void Log(ELog level, const char* msg, const float arg);
void Log(ELog level, const char* msg, const int64_t arg);
void Log(ELog level, const char* msg, const uint64_t arg);
void Log(ELog level, const char* msg, const godot::String& s);
void Log(ELog level, const char* msg, const godot::Vector2& v2);
void Log(ELog level, const char* msg, const godot::Vector3& v2);
void Log(ELog level, const char* msg, const godot::Quaternion& q);
void Log(ELog level, const char* msg, const godot::Transform3D& t);
