#ifndef GD_CORECORE_PLUGIN_GAMEPLAY_H
#define GD_CORECORE_PLUGIN_GAMEPLAY_H

#include "godotincludes.h"
#include "timer.hpp"
#include "math.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <typeinfo>
#include <random>

#define ASSERT(expr, msg)                                                                                              \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, msg); }
#define ASSERT_NOTNULL(ptr) ASSERT(ptr != nullptr, "")
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);

// Distinction between editor-mode and in-game
#define RETURN_IF_EDITOR                                                                                               \
	if (godot::Engine::get_singleton()->is_editor_hint()) { return; }

#define GETNAME(class_name)                                                                                            \
	String get_class_name() const { return #class_name; }
#define DEFAULT_PROPERTY(class_name)                                                                                   \
	godot::ClassDB::bind_method(D_METHOD("get_class_name"), &class_name::get_class_name);                              \
	godot::ClassDB::add_property(#class_name,                                                                          \
			PropertyInfo(Variant::STRING, "class_name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "",           \
			"get_class_name");

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

// From the context of the MainScene
namespace nodePaths {
	constexpr const char* root = "/root";
	constexpr const char* player = "/root/Mainscene/PlayerNode";
	constexpr const char* camera_pivot = "/root/Mainscene/PlayerNode/CameraPivot";
	constexpr const char* camera = ("%s/Camera", camera_pivot);

} //namespace nodePaths


#endif // GD_CORECORE_PLUGIN_GAMEPLAY_H