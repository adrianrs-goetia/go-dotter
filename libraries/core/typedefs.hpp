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
inline constexpr uint32_t staticWorld = 1;
inline constexpr uint32_t dynamicWorld = 2;
inline constexpr uint32_t pickableObjects = 3;
inline constexpr uint32_t wallWalking = 4;
inline constexpr uint32_t attackTarget = 5;
inline constexpr uint32_t grapplingTarget = 7;
inline constexpr uint32_t parryTarget = 8;
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
