#ifndef GD_CORECORE_PLUGIN_GAMEPLAY_H
#define GD_CORECORE_PLUGIN_GAMEPLAY_H

#include "godotincludes.h"
#include <chrono>
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

class Timestamp {
	std::chrono::system_clock::time_point timestamp;

public:
	Timestamp() : timestamp(std::chrono::system_clock::now()) {}
	bool timestamp_within_timeframe(float timeframe_seconds) {
		float duration_since_timestamp =
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timestamp)
						.count();
		float sec = (duration_since_timestamp / 1e9);
		return (duration_since_timestamp / 1e9) < timeframe_seconds;
	}
};

// Directions
static const godot::Vector3 g_up(0, 1, 0);
static const godot::Vector3 g_forward(0, 0, 1);
static const godot::Vector3 g_right(1, 0, 0);

// From the context of the MainScene
namespace NodePaths {

	constexpr const char* player = "/root/Mainscene/PlayerNode";
	constexpr const char* camera_pivot = "/root/Mainscene/PlayerNode/CameraPivot";
	constexpr const char* camera = ("%s/Camera", camera_pivot);

} //namespace NodePaths

// Math constants
constexpr float PI = 3.14159f;
constexpr float PI_HALF = 3.14159f / 2.f;
constexpr float PI_TWO = 3.14159f * 2.f;


inline float canonical_random_number(float min, float max) {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dis(min, max);
	return dis(rng);
}

#endif // GD_CORECORE_PLUGIN_GAMEPLAY_H