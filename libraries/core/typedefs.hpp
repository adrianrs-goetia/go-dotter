#pragma once

#include <cstdint>

// From the context of the MainScene
namespace nodePaths {
constexpr const char* root = "/root";
constexpr const char* player = "/root/Mainscene/PlayerNode";
constexpr const char* cameraPivot = "/root/Mainscene/PlayerNode/CameraPivot";
constexpr const char* camera = ("%s/Camera", cameraPivot);

} //namespace nodePaths

namespace collisionflags {
inline constexpr auto staticWorld = 1;
inline constexpr auto dynamicWorld = 2;
inline constexpr auto player = 3;
inline constexpr auto pickableObjects = 4;
inline constexpr auto wallWalking = 5;
inline constexpr auto attackTarget = 17;
inline constexpr auto grapplingTarget = 18;
inline constexpr auto parryTarget = 19;
} //namespace collisionflags

namespace godotgroups {
inline constexpr auto enemies = "enemies";
inline constexpr auto projectile = "projectile";
}; //namespace godotgroups

/**
 * Custom notifications
 * Have to avoid collision with ones defined in godot-cpp node.hpp
 */
enum ENotifications : int {
	DESTROY = 3000,
	PARRIED = 3001,
	ATTACKED = 3002,
};
