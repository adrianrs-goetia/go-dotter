#pragma once

#include <cstdint>

// From the context of the MainScene
namespace nodePaths {
	constexpr const char* root = "/root";
	constexpr const char* player = "/root/Mainscene/PlayerNode";
	constexpr const char* camera_pivot = "/root/Mainscene/PlayerNode/CameraPivot";
	constexpr const char* camera = ("%s/Camera", camera_pivot);

} //namespace nodePaths

namespace collisionflags {
	static constexpr uint32_t staticWorld = 1;
	static constexpr uint32_t dynamicWorld = 2;
	static constexpr uint32_t pickableObjects = 3;
	static constexpr uint32_t wallWalking = 4;
	static constexpr uint32_t grapplingTarget = 7;
	static constexpr uint32_t parryTarget = 8;
} //namespace collisionflags

