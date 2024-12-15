#pragma once

#include "../typedefs.hpp"

#include <managers/inputManager.h>

#include <configHandler.h>

#define CONFIG_PREFIX "player"

namespace fsm::player::utils {

inline void movement_acceleration(Context& context, float acceleration, float deceleration, float delta) {
	// direction
	if (context.input->m_inputRaw.abs() > godot::Vector2()) {
		context.physics.velocity.x = godot::Math::move_toward(context.physics.velocity.x,
			context.input->m_inputCameraRelative.x * GETPARAM_F("walkSpeed"),
			acceleration * delta);
		context.physics.velocity.z = godot::Math::move_toward(context.physics.velocity.z,
			context.input->m_inputCameraRelative.y * GETPARAM_F("walkSpeed"),
			acceleration * delta);
	}
	else {
		context.physics.velocity.x = godot::Math::move_toward(context.physics.velocity.x, 0.0f, deceleration * delta);
		context.physics.velocity.z = godot::Math::move_toward(context.physics.velocity.z, 0.0f, deceleration * delta);
	}
}

} //namespace fsm::player::utils

#undef CONFIG_PREFIX
