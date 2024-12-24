#pragma once

#include "../typedefs.hpp"

#include <managers/inputManager.h>

#include <godot_cpp/classes/kinematic_collision3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

#include <configHandler.h>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player"

namespace fsm::player::utils {

inline void movementAcceleration(Context& context, float acceleration, float deceleration, float delta) {
	auto& vec = context.physics.movement;
	// direction
	// if (context.input->m_inputRaw.abs() > godot::Vector2()) {
	float desiredSpeed =
		context.input->isActionHeld(EInputAction::SPRINT) ? GETPARAM_F("sprintSpeed") : GETPARAM_F("walkSpeed");

	if (context.input->m_inputRaw.length_squared() > 0.2f) {
		vec.x = godot::Math::move_toward(vec.x, context.input->m_inputCameraRelative.x * desiredSpeed, acceleration);
		vec.z = godot::Math::move_toward(vec.z, context.input->m_inputCameraRelative.y * desiredSpeed, acceleration);
	}
	else {
		vec.x = godot::Math::move_toward(vec.x, 0.0f, deceleration);
		vec.z = godot::Math::move_toward(vec.z, 0.0f, deceleration);
	}
}

typedef struct {
	uint32_t layer;
	uint32_t mask;
} CollisionLayerMask;

inline CollisionLayerMask disableCollision(Context& context) {
	CollisionLayerMask clm{ context.owner->get_collision_layer(), context.owner->get_collision_mask() };
	context.owner->set_collision_layer(0);
	context.owner->set_collision_mask(1); // Not masking out static world
	return clm;
}

inline void enableCollision(Context& context, const CollisionLayerMask& clm) {
	context.owner->set_collision_layer(clm.layer);
	context.owner->set_collision_mask(clm.mask);
}

} //namespace fsm::player::utils

#undef CONFIG_PREFIX
